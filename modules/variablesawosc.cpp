#include "dsp.h"
#include "variablesawosc.h"
#include <math.h>

using namespace daisysp;

void VariableSawOscillator::Init(float sample_rate)
{
    sample_rate_ = sample_rate;

    phase_       = 0.0f;
    next_sample_ = 0.0f;
    previous_pw_ = 0.5f;
    high_        = false;

    SetFreq(220.f);
    SetPW(0.f);
    SetWaveshape(1.f);
}

float VariableSawOscillator::Process()
{
    float next_sample = next_sample_;

    float this_sample = next_sample;
    next_sample       = 0.0f;

    const float triangle_amount = waveshape_;
    const float notch_amount    = 1.0f - waveshape_;
    const float slope_up        = 1.0f / (pw_);
    const float slope_down      = 1.0f / (1.0f - pw_);

    phase_ += frequency_;

    if(!high_ && phase_ >= pw_)
    {
        const float triangle_step
            = (slope_up + slope_down) * frequency_ * triangle_amount;
        const float notch
            = (kVariableSawNotchDepth + 1.0f - pw_) * notch_amount;
        const float t = (phase_ - pw_) / (previous_pw_ - pw_ + frequency_);
        this_sample += notch * ThisBlepSample(t);
        next_sample += notch * NextBlepSample(t);
        this_sample -= triangle_step * ThisIntegratedBlepSample(t);
        next_sample -= triangle_step * NextIntegratedBlepSample(t);
        high_ = true;
    }
    else if(phase_ >= 1.0f)
    {
        phase_ -= 1.0f;
        const float triangle_step
            = (slope_up + slope_down) * frequency_ * triangle_amount;
        const float notch = (kVariableSawNotchDepth + 1.0f) * notch_amount;
        const float t     = phase_ / frequency_;
        this_sample -= notch * ThisBlepSample(t);
        next_sample -= notch * NextBlepSample(t);
        this_sample += triangle_step * ThisIntegratedBlepSample(t);
        next_sample += triangle_step * NextIntegratedBlepSample(t);
        high_ = false;
    }

    next_sample += ComputeNaiveSample(
        phase_, pw_, slope_up, slope_down, triangle_amount, notch_amount);
    previous_pw_ = pw_;

    next_sample_ = next_sample;
    return (2.0f * this_sample - 1.0f) / (1.0f + kVariableSawNotchDepth);
}

void VariableSawOscillator::SetFreq(float frequency)
{
    frequency  = frequency / sample_rate_;
    frequency  = frequency >= .25f ? .25f : frequency;
    pw_        = frequency >= .25f ? .5f : pw_;
    frequency_ = frequency;
}

void VariableSawOscillator::SetPW(float pw)
{
    if(frequency_ >= .25f)
    {
        pw_ = .5f;
    }
    else
    {
        pw_ = fclamp(pw, frequency_ * 2.0f, 1.0f - 2.0f * frequency_);
    }
}

void VariableSawOscillator::SetWaveshape(float waveshape)
{
    waveshape_ = waveshape;
}

float VariableSawOscillator::ComputeNaiveSample(float phase,
                                                float pw,
                                                float slope_up,
                                                float slope_down,
                                                float triangle_amount,
                                                float notch_amount)
{
    float notch_saw = phase < pw ? phase : 1.0f + kVariableSawNotchDepth;
    float triangle
        = phase < pw ? phase * slope_up : 1.0f - (phase - pw) * slope_down;
    return notch_saw * notch_amount + triangle * triangle_amount;
}