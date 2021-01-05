#include "dsp.h"
#include "variableshapeosc.h"
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

    SetFreq(440.f);
    SetWaveshape(0.f);
    SetPW(0.f);
    SetSync(false);
    SetSyncFreq(220.f);
}

float VariableShapeOscillator::Process()
{
    float next_sample = next_sample_;

    bool  reset                   = false;
    bool  transition_during_reset = false;
    float reset_time              = 0.0f;

    float this_sample = next_sample;
    next_sample       = 0.0f;

    const float square_amount   = fmax(waveshape_ - 0.5f, 0.0f) * 2.0f;
    const float triangle_amount = fmax(1.0f - waveshape_ * 2.0f, 0.0f);
    const float slope_up        = 1.0f / (pw_);
    const float slope_down      = 1.0f / (1.0f - pw_);

    if(enable_sync_)
    {
        master_phase_ += master_frequency_;
        if(master_phase_ >= 1.0f)
        {
            master_phase_ -= 1.0f;
            reset_time = master_phase_ / master_frequency_;

            float slave_phase_at_reset
                = slave_phase_ + (1.0f - reset_time) * slave_frequency_;
            reset = true;
            if(slave_phase_at_reset >= 1.0f)
            {
                slave_phase_at_reset -= 1.0f;
                transition_during_reset = true;
            }
            if(!high_ && slave_phase_at_reset >= pw_)
            {
                transition_during_reset = true;
            }
            float value = ComputeNaiveSample(slave_phase_at_reset,
                                             pw_,
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
            if(slave_phase_ < pw_)
            {
                break;
            }
            float t = (slave_phase_ - pw_)
                      / (previous_pw_ - pw_ + slave_frequency_);
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

    if(enable_sync_ && reset)
    {
        slave_phase_ = reset_time * slave_frequency_;
        high_        = false;
    }

    next_sample += ComputeNaiveSample(slave_phase_,
                                      pw_,
                                      slope_up,
                                      slope_down,
                                      triangle_amount,
                                      square_amount);
    previous_pw_ = pw_;


    next_sample_ = next_sample;
    return (2.0f * this_sample - 1.0f);
}

void VariableShapeOscillator::SetFreq(float frequency)
{
    frequency         = frequency / sample_rate_;
    frequency         = frequency >= .25f ? .25f : frequency;
    master_frequency_ = frequency;
}

void VariableShapeOscillator::SetPW(float pw)
{
    if(slave_frequency_ >= .25f)
    {
        pw_ = .5f;
    }
    else
    {
        pw_ = fclamp(
            pw, slave_frequency_ * 2.0f, 1.0f - 2.0f * slave_frequency_);
    }
}

void VariableShapeOscillator::SetWaveshape(float waveshape)
{
    waveshape_ = waveshape;
}

void VariableShapeOscillator::SetSync(bool enable_sync)
{
    enable_sync_ = enable_sync;
}

void VariableShapeOscillator::SetSyncFreq(float frequency)
{
    frequency        = frequency / sample_rate_;
    pw_              = frequency >= .25f ? .5f : pw_;
    frequency        = frequency >= .25f ? .25f : frequency;
    slave_frequency_ = frequency;
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