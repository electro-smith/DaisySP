#include "dsp.h"
#include "oscillatorbank.h"
#include <math.h>

using namespace daisysp;

void OscillatorBank::Init(float sample_rate)
{
    sample_rate_ = sample_rate;

    phase_       = 0.0f;
    next_sample_ = 0.0f;
    segment_     = 0.0f;

    frequency_  = 0.f;
    saw_8_gain_ = 0.0f;
    saw_4_gain_ = 0.0f;
    saw_2_gain_ = 0.0f;
    saw_1_gain_ = 0.0f;

    recalc_ = recalc_gain_ = true;
    SetGain(1.f);

    for(int i = 0; i < 7; i++)
    {
        registration_[i]           = 0.f;
        unshifted_registration_[i] = 0.f;
    }
    SetSingleAmp(1.f, 0);
    SetFreq(440.f);
}

float OscillatorBank::Process()
{
    if(recalc_)
    {
        recalc_ = false;
        frequency_ *= 8.0f;

        // Deal with very high frequencies by shifting everything 1 or 2 octave
        // down: Instead of playing the 1st harmonic of a 8kHz wave, we play the
        // second harmonic of a 4kHz wave.
        size_t shift = 0;
        while(frequency_ > 0.5f)
        {
            shift += 2;
            frequency_ *= 0.5f;
        }

        for(int i = 0; i < 7; i++)
        {
            registration_[i] = 0.f;
        }

        for(size_t i = 0; i < 7 - shift; i++)
        {
            registration_[i + shift] = unshifted_registration_[i];
        }
    }

    if(recalc_gain_ || recalc_)
    {
        saw_8_gain_ = (registration_[0] + 2.0f * registration_[1]) * gain_;
        saw_4_gain_
            = (registration_[2] - registration_[1] + 2.0f * registration_[3])
              * gain_;
        saw_2_gain_
            = (registration_[4] - registration_[3] + 2.0f * registration_[5])
              * gain_;
        saw_1_gain_ = (registration_[6] - registration_[5]) * gain_;
    }

    float this_sample_ = next_sample_;
    next_sample_       = 0.0f;

    phase_ += frequency_;
    int next_segment_ = static_cast<int>(phase_);
    if(next_segment_ != segment_)
    {
        float discontinuity = 0.0f;
        if(next_segment_ == 8)
        {
            phase_ -= 8.0f;
            next_segment_ -= 8;
            discontinuity -= saw_8_gain_;
        }
        if((next_segment_ & 3) == 0)
        {
            discontinuity -= saw_4_gain_;
        }
        if((next_segment_ & 1) == 0)
        {
            discontinuity -= saw_2_gain_;
        }
        discontinuity -= saw_1_gain_;
        if(discontinuity != 0.0f)
        {
            float fraction = phase_ - static_cast<float>(next_segment_);
            float t        = fraction / frequency_;
            this_sample_ += ThisBlepSample(t) * discontinuity;
            next_sample_ += NextBlepSample(t) * discontinuity;
        }
    }
    segment_ = next_segment_;

    next_sample_ += (phase_ - 4.0f) * saw_8_gain_ * 0.125f;
    next_sample_ += (phase_ - float(segment_ & 4) - 2.0f) * saw_4_gain_ * 0.25f;
    next_sample_ += (phase_ - float(segment_ & 6) - 1.0f) * saw_2_gain_ * 0.5f;
    next_sample_ += (phase_ - float(segment_ & 7) - 0.5f) * saw_1_gain_;

    return 2.0f * this_sample_;
}

void OscillatorBank::SetFreq(float freq)
{
    freq       = freq / sample_rate_;
    freq       = freq > 0.5f ? 0.5f : freq;
    recalc_    = cmp(freq, frequency_) || recalc_;
    frequency_ = freq;
}

void OscillatorBank::SetAmplitudes(const float* amplitudes)
{
    for(int i = 0; i < 7; i++)
    {
        recalc_ = cmp(unshifted_registration_[i], amplitudes[i]) || recalc_;
        unshifted_registration_[i] = amplitudes[i];
    }
}

void OscillatorBank::SetSingleAmp(float amp, int idx)
{
    if(idx < 0 || idx > 6)
    {
        return;
    }
    recalc_ = cmp(unshifted_registration_[idx], amp) || recalc_;
    unshifted_registration_[idx] = amp;
}

void OscillatorBank::SetGain(float gain)
{
    gain         = gain > 1.f ? 1.f : gain;
    gain         = gain < 0.f ? 0.f : gain;
    recalc_gain_ = cmp(gain, gain_) || recalc_gain_;
    gain_        = gain;
}