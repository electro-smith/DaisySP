#include "dsp.h"
#include "syntheticsnaredrum.h"
#include <math.h>

using namespace daisysp;

void SyntheticSnareDrum::Init()
{
    phase_[0]        = 0.0f;
    phase_[1]        = 0.0f;
    drum_amplitude_  = 0.0f;
    snare_amplitude_ = 0.0f;
    fm_              = 0.0f;
    hold_counter_    = 0;
    sustain_gain_    = 0.0f;

    drum_lp_.Init();
    snare_hp_.Init();
    snare_lp_.Init();
}

inline float SyntheticSnareDrum::DistortedSine(float phase)
{
    float triangle = (phase < 0.5f ? phase : 1.0f - phase) * 4.0f - 1.3f;
    return 2.0f * triangle / (1.0f + fabsf(triangle));
}

void SyntheticSnareDrum::Process(bool   sustain,
            bool   trigger,
            float  accent,
            float  f0,
            float  fm_amount,
            float  decay,
            float  snappy,
            float* out,
            size_t size)
{
    const float decay_xt = decay * (1.0f + decay * (decay - 1.0f));
    fm_amount *= fm_amount;
    const float drum_decay
        = 1.0f
          - 1.0f / (0.015f * kSampleRate)
                * stmlib::SemitonesToRatio(-decay_xt * 72.0f - fm_amount * 12.0f
                                           + snappy * 7.0f);
    const float snare_decay
        = 1.0f
          - 1.0f / (0.01f * kSampleRate)
                * stmlib::SemitonesToRatio(-decay * 60.0f - snappy * 7.0f);
    const float fm_decay = 1.0f - 1.0f / (0.007f * kSampleRate);

    snappy = snappy * 1.1f - 0.05f;
    CONSTRAIN(snappy, 0.0f, 1.0f);

    const float drum_level  = stmlib::Sqrt(1.0f - snappy);
    const float snare_level = stmlib::Sqrt(snappy);

    const float snare_f_min = std::min(10.0f * f0, 0.5f);
    const float snare_f_max = std::min(35.0f * f0, 0.5f);

    snare_hp_.set_f<stmlib::FREQUENCY_FAST>(snare_f_min);
    snare_lp_.set_f_q<stmlib::FREQUENCY_FAST>(snare_f_max,
                                              0.5f + 2.0f * snappy);
    drum_lp_.set_f<stmlib::FREQUENCY_FAST>(3.0f * f0);

    if(trigger)
    {
        snare_amplitude_ = drum_amplitude_ = 0.3f + 0.7f * accent;
        fm_                                = 1.0f;
        phase_[0] = phase_[1] = 0.0f;
        hold_counter_ = static_cast<int>((0.04f + decay * 0.03f) * kSampleRate);
    }

    stmlib::ParameterInterpolator sustain_gain(
        &sustain_gain_, accent * decay, size);
    while(size--)
    {
        if(sustain)
        {
            snare_amplitude_ = sustain_gain.Next();
            drum_amplitude_  = snare_amplitude_;
            fm_              = 0.0f;
        }
        else
        {
            // Compute all D envelopes.
            // The envelope for the drum has a very long tail.
            // The envelope for the snare has a "hold" stage which lasts between
            // 40 and 70 ms
            drum_amplitude_
                *= (drum_amplitude_ > 0.03f || !(size & 1)) ? drum_decay : 1.0f;
            if(hold_counter_)
            {
                --hold_counter_;
            }
            else
            {
                snare_amplitude_ *= snare_decay;
            }
            fm_ *= fm_decay;
        }

        // The 909 circuit has a funny kind of oscillator coupling - the signal
        // leaving Q40's collector and resetting all oscillators allow some
        // intermodulation.
        float reset_noise        = 0.0f;
        float reset_noise_amount = (0.125f - f0) * 8.0f;
        CONSTRAIN(reset_noise_amount, 0.0f, 1.0f);
        reset_noise_amount *= reset_noise_amount;
        reset_noise_amount *= fm_amount;
        reset_noise += phase_[0] > 0.5f ? -1.0f : 1.0f;
        reset_noise += phase_[1] > 0.5f ? -1.0f : 1.0f;
        reset_noise *= reset_noise_amount * 0.025f;

        float f = f0 * (1.0f + fm_amount * (4.0f * fm_));
        phase_[0] += f;
        phase_[1] += f * 1.47f;
        if(reset_noise_amount > 0.1f)
        {
            if(phase_[0] >= 1.0f + reset_noise)
            {
                phase_[0] = 1.0f - phase_[0];
            }
            if(phase_[1] >= 1.0f + reset_noise)
            {
                phase_[1] = 1.0f - phase_[1];
            }
        }
        else
        {
            if(phase_[0] >= 1.0f)
            {
                phase_[0] -= 1.0f;
            }
            if(phase_[1] >= 1.0f)
            {
                phase_[1] -= 1.0f;
            }
        }

        float drum = -0.1f;
        drum += DistortedSine(phase_[0]) * 0.60f;
        drum += DistortedSine(phase_[1]) * 0.25f;
        drum *= drum_amplitude_ * drum_level;
        drum = drum_lp_.Process<stmlib::FILTER_MODE_LOW_PASS>(drum);

        float noise = stmlib::Random::GetFloat();
        float snare = snare_lp_.Process<stmlib::FILTER_MODE_LOW_PASS>(noise);
        snare       = snare_hp_.Process<stmlib::FILTER_MODE_HIGH_PASS>(snare);
        snare       = (snare + 0.1f) * (snare_amplitude_ + fm_) * snare_level;

        *out++ = snare + drum; // It's a snare, it's a drum, it's a snare drum.
    }
}
