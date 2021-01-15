#include <random>
#include "dsp.h"
#include "clockednoise.h"

using namespace daisysp;

void ClockedNoise::Init(float sample_rate)
{
    sample_rate_ = sample_rate;

    phase_       = 0.0f;
    sample_      = 0.0f;
    next_sample_ = 0.0f;
    frequency_   = 0.001f;
}

float ClockedNoise::Process()
{
    float next_sample = next_sample_;
    float sample      = sample_;

    float this_sample = next_sample;
    next_sample       = 0.0f;

    const float raw_sample = rand() * kRandFrac * 2.0f - 1.0f;
    float       raw_amount = 4.0f * (frequency_ - 0.25f);
    raw_amount             = fclamp(raw_amount, 0.0f, 1.0f);

    phase_ += frequency_;

    if(phase_ >= 1.0f)
    {
        phase_ -= 1.0f;
        float t             = phase_ / frequency_;
        float new_sample    = raw_sample;
        float discontinuity = new_sample - sample;
        this_sample += discontinuity * ThisBlepSample(t);
        next_sample += discontinuity * NextBlepSample(t);
        sample = new_sample;
    }

    next_sample += sample;
    next_sample_ = next_sample;
    sample_      = sample;

    return this_sample + raw_amount * (raw_sample - this_sample);
}

void ClockedNoise::SetFreq(float freq)
{
    freq       = freq / sample_rate_;
    freq       = fclamp(freq, 0.0f, 1.0f);
    frequency_ = freq;
}

void ClockedNoise::Sync()
{
    phase_ = 1.0f;
}