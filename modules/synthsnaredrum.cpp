#include "dsp.h"
#include "synthsnaredrum.h"
#include <math.h>
#include <stdlib.h>

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

    SetSustain(false);
    SetAccent(.6f);
    SetFreq(200.f);
    SetFmAmount(.1f);
    SetDecay(.3f);
    SetSnappy(.7f);

    trig_ = false;

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

float SyntheticSnareDrum::Process(bool trigger)
{
    const float decay_xt = decay_ * (1.0f + decay_ * (decay_ - 1.0f));
    const float drum_decay
        = 1.0f
          - 1.0f / (0.015f * sample_rate_)
                * powf(2.f,
                       kOneTwelfth
                           * (-decay_xt * 72.0f - fm_amount_ * 12.0f
                              + snappy_ * 7.0f));

    const float snare_decay
        = 1.0f
          - 1.0f / (0.01f * sample_rate_)
                * powf(2.f, kOneTwelfth * (-decay_ * 60.0f - snappy_ * 7.0f));
    const float fm_decay = 1.0f - 1.0f / (0.007f * sample_rate_);

    float snappy = snappy_ * 1.1f - 0.05f;
    snappy       = fclamp(snappy, 0.0f, 1.0f);

    const float drum_level  = sqrtf(1.0f - snappy);
    const float snare_level = sqrtf(snappy);

    const float snare_f_min = fmin(10.0f * f0_, 0.5f);
    const float snare_f_max = fmin(35.0f * f0_, 0.5f);

    snare_hp_.SetFreq(snare_f_min * sample_rate_);
    snare_lp_.SetFreq(snare_f_max * sample_rate_);
    snare_lp_.SetRes(0.5f + 2.0f * snappy);

    drum_lp_.SetFreq(3.0f * f0_ * sample_rate_);

    if(trigger || trig_)
    {
        trig_            = false;
        snare_amplitude_ = drum_amplitude_ = 0.3f + 0.7f * accent_;
        fm_                                = 1.0f;
        phase_[0] = phase_[1] = 0.0f;
        hold_counter_
            = static_cast<int>((0.04f + decay_ * 0.03f) * sample_rate_);
    }

    even = !even;
    if(sustain_)
    {
        sustain_gain_ = snare_amplitude_ = accent_ * decay_;
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
    float reset_noise_amount = (0.125f - f0_) * 8.0f;
    reset_noise_amount       = fclamp(reset_noise_amount, 0.0f, 1.0f);
    reset_noise_amount *= reset_noise_amount;
    reset_noise_amount *= fm_amount_;
    reset_noise += phase_[0] > 0.5f ? -1.0f : 1.0f;
    reset_noise += phase_[1] > 0.5f ? -1.0f : 1.0f;
    reset_noise *= reset_noise_amount * 0.025f;

    float f = f0_ * (1.0f + fm_amount_ * (4.0f * fm_));
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

    float noise = rand() * kRandFrac;
    snare_lp_.Process(noise);
    float snare = snare_lp_.Low();
    snare_hp_.Process(snare);
    snare = snare_hp_.High();
    snare = (snare + 0.1f) * (snare_amplitude_ + fm_) * snare_level;

    return snare + drum; // It's a snare, it's a drum, it's a snare drum.
}

void SyntheticSnareDrum::Trig()
{
    trig_ = true;
}

void SyntheticSnareDrum::SetSustain(bool sustain)
{
    sustain_ = sustain;
}

void SyntheticSnareDrum::SetAccent(float accent)
{
    accent_ = fclamp(accent, 0.f, 1.f);
}

void SyntheticSnareDrum::SetFreq(float f0)
{
    f0 /= sample_rate_;
    f0_ = fclamp(f0, 0.f, 1.f);
}

void SyntheticSnareDrum::SetFmAmount(float fm_amount)
{
    fm_amount  = fclamp(fm_amount, 0.f, 1.f);
    fm_amount_ = fm_amount * fm_amount;
}

void SyntheticSnareDrum::SetDecay(float decay)
{
    decay_ = fmax(decay, 0.f);
}

void SyntheticSnareDrum::SetSnappy(float snappy)
{
    snappy_ = fclamp(snappy, 0.f, 1.f);
}
