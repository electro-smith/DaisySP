#include "dsp.h"
#include "variableosc.h"
#include <math.h>

using namespace daisysp;

void VariableShapeOscillator::Init(float sample_rate)
{
    sample_rate_ = sample_rate;

    master_phase_ = 0.0f;
    slave_phase_  = 0.0f;
    next_sample_  = 0.0f;
    previous_pw_  = 0.5f;
    high_         = false;

    master_frequency_ = 0.0f;
    slave_frequency_  = 0.01f;
    pw_               = 0.5f;
    waveshape_        = 0.0f;
}

template <bool enable_sync>
float VariableShapeOscillator::Process(float master_frequency,
                                       float frequency,
                                       float pw,
                                       float waveshape)
{
    float kMaxFrequency = .25f;

    if(master_frequency >= kMaxFrequency)
    {
        master_frequency = kMaxFrequency;
    }
    if(frequency >= kMaxFrequency)
    {
        frequency = kMaxFrequency;
    }

    if(frequency >= 0.25f)
    {
        pw = 0.5f;
    }
    else
    {
        pw = fclamp(pw, frequency * 2.0f, 1.0f - 2.0f * frequency);
    }

    float next_sample = next_sample_;

    bool  reset                   = false;
    bool  transition_during_reset = false;
    float reset_time              = 0.0f;

    float this_sample = next_sample;
    next_sample       = 0.0f;

    master_frequency_           = master_frequency;
    slave_frequency_            = frequency;
    pw_                         = pw;
    waveshape_                  = waveshape;
    const float square_amount   = fmax(waveshape - 0.5f, 0.0f) * 2.0f;
    const float triangle_amount = fmax(1.0f - waveshape * 2.0f, 0.0f);
    const float slope_up        = 1.0f / (pw);
    const float slope_down      = 1.0f / (1.0f - pw);

    if(enable_sync)
    {
        master_phase_ += master_frequency;
        if(master_phase_ >= 1.0f)
        {
            master_phase_ -= 1.0f;
            reset_time = master_phase_ / master_frequency;

            float slave_phase_at_reset
                = slave_phase_ + (1.0f - reset_time) * slave_frequency_;
            reset = true;
            if(slave_phase_at_reset >= 1.0f)
            {
                slave_phase_at_reset -= 1.0f;
                transition_during_reset = true;
            }
            if(!high_ && slave_phase_at_reset >= pw)
            {
                transition_during_reset = true;
            }
            float value = ComputeNaiveSample(slave_phase_at_reset,
                                             pw,
                                             slope_up,
                                             slope_down,
                                             triangle_amount,
                                             square_amount);
            this_sample -= value * ThisBlepSample(reset_time);
            next_sample -= value * NextBlepSample(reset_time);
        }
    }

    slave_phase_ += slave_frequency_;
    while(transition_during_reset || !reset)
    {
        if(!high_)
        {
            if(slave_phase_ < pw)
            {
                break;
            }
            float t
                = (slave_phase_ - pw) / (previous_pw_ - pw + slave_frequency_);
            float triangle_step = (slope_up + slope_down) * slave_frequency_;
            triangle_step *= triangle_amount;

            this_sample += square_amount * ThisBlepSample(t);
            next_sample += square_amount * NextBlepSample(t);
            this_sample -= triangle_step * ThisIntegratedBlepSample(t);
            next_sample -= triangle_step * NextIntegratedBlepSample(t);
            high_ = true;
        }

        if(high_)
        {
            if(slave_phase_ < 1.0f)
            {
                break;
            }
            slave_phase_ -= 1.0f;
            float t             = slave_phase_ / slave_frequency_;
            float triangle_step = (slope_up + slope_down) * slave_frequency_;
            triangle_step *= triangle_amount;

            this_sample -= (1.0f - triangle_amount) * ThisBlepSample(t);
            next_sample -= (1.0f - triangle_amount) * NextBlepSample(t);
            this_sample += triangle_step * ThisIntegratedBlepSample(t);
            next_sample += triangle_step * NextIntegratedBlepSample(t);
            high_ = false;
        }
    }

    if(enable_sync && reset)
    {
        slave_phase_ = reset_time * slave_frequency_;
        high_        = false;
    }

    next_sample += ComputeNaiveSample(
        slave_phase_, pw, slope_up, slope_down, triangle_amount, square_amount);
    previous_pw_ = pw;


    next_sample_ = next_sample;
    return (2.0f * this_sample - 1.0f);
}


float VariableShapeOscillator::ComputeNaiveSample(float phase,
                                                  float pw,
                                                  float slope_up,
                                                  float slope_down,
                                                  float triangle_amount,
                                                  float square_amount)
{
    float saw    = phase;
    float square = phase < pw ? 0.0f : 1.0f;
    float triangle
        = phase < pw ? phase * slope_up : 1.0f - (phase - pw) * slope_down;
    saw += (square - saw) * square_amount;
    saw += (triangle - saw) * triangle_amount;
    return saw;
}

float VariableShapeOscillator::ThisBlepSample(float t)
{
    return 0.5f * t * t;
}

float VariableShapeOscillator::NextBlepSample(float t)
{
    t = 1.0f - t;
    return -0.5f * t * t;
}

float VariableShapeOscillator::NextIntegratedBlepSample(float t)
{
    const float t1 = 0.5f * t;
    const float t2 = t1 * t1;
    const float t4 = t2 * t2;
    return 0.1875f - t1 + 1.5f * t2 - t4;
}

float VariableShapeOscillator::ThisIntegratedBlepSample(float t)
{
    return NextIntegratedBlepSample(1.0f - t);
}