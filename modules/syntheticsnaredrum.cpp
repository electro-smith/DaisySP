#include "dsp.h"
#include "syntheticsnaredrum.h"
#include <math.h>

using namespace daisysp;

void SyntheticSnareDrum::Init(float sample_rate)
{
    sample_rate_ = sample_rate;

    phase_[0]        = 0.0f;
    phase_[1]        = 0.0f;
    drum_amplitude_  = 0.0f;
    snare_amplitude_ = 0.0f;
    fm_              = 0.0f;
    hold_counter_    = 0;
    sustain_gain_    = 0.0f;

    drum_lp_.Init(sample_rate_);
    snare_hp_.Init(sample_rate_);
    snare_lp_.Init(sample_rate_);
}

inline float SyntheticSnareDrum::DistortedSine(float phase)
{
    float triangle = (phase < 0.5f ? phase : 1.0f - phase) * 4.0f - 1.3f;
    return 2.0f * triangle / (1.0f + fabsf(triangle));
}

bool even = true;

float SyntheticSnareDrum::Process(bool  sustain,
                                  bool  trigger,
                                  float accent,
                                  float f0,
                                  float fm_amount,
                                  float decay,
                                  float snappy)
{
    const float decay_xt = decay * (1.0f + decay * (decay - 1.0f));
    fm_amount *= fm_amount;
    const float drum_decay
        = 1.0f
          - 1.0f / (0.015f * sample_rate_)
                * powf(2.f,
                       ratio_frac_
                           * (-decay_xt * 72.0f - fm_amount * 12.0f
                              + snappy * 7.0f));

    const float snare_decay
        = 1.0f
          - 1.0f / (0.01f * sample_rate_)
                * powf(2.f, ratio_frac_ * (-decay * 60.0f - snappy * 7.0f));
    const float fm_decay = 1.0f - 1.0f / (0.007f * sample_rate_);

    snappy = snappy * 1.1f - 0.05f;
    snappy = fclamp(snappy, 0.0f, 1.0f);

    const float drum_level  = sqrtf(1.0f - snappy);
    const float snare_level = sqrtf(snappy);

    const float snare_f_min = fmin(10.0f * f0, 0.5f);
    const float snare_f_max = fmin(35.0f * f0, 0.5f);

    snare_hp_.SetFreq(snare_f_min * sample_rate_);
    snare_lp_.SetFreq(snare_f_max * sample_rate_);
    snare_lp_.SetRes(0.5f + 2.0f * snappy);

    drum_lp_.SetFreq(3.0f * f0 * sample_rate_);

    if(trigger)
    {
        snare_amplitude_ = drum_amplitude_ = 0.3f + 0.7f * accent;
        fm_                                = 1.0f;
        phase_[0] = phase_[1] = 0.0f;
        hold_counter_
            = static_cast<int>((0.04f + decay * 0.03f) * sample_rate_);
    }

    even = !even;
    if(sustain)
    {
        sustain_gain_ = snare_amplitude_ = accent * decay;
        drum_amplitude_                  = snare_amplitude_;
        fm_                              = 0.0f;
    }
    else
    {
        // Compute all D envelopes.
        // The envelope for the drum has a very long tail.
        // The envelope for the snare has a "hold" stage which lasts between
        // 40 and 70 ms
        drum_amplitude_
            *= (drum_amplitude_ > 0.03f || even) ? drum_decay : 1.0f;
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
    reset_noise_amount       = fclamp(reset_noise_amount, 0.0f, 1.0f);
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

    drum_lp_.Process(drum);
    drum = drum_lp_.Low();

    float noise = random() * rand_frac_;
    snare_lp_.Process(noise);
    float snare = snare_lp_.Low();
    snare_hp_.Process(snare);
    snare = snare_hp_.High();
    snare = (snare + 0.1f) * (snare_amplitude_ + fm_) * snare_level;

    return snare + drum; // It's a snare, it's a drum, it's a snare drum.
}
