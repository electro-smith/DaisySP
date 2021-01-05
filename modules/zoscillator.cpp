#include "dsp.h"
#include "zoscillator.h"
#include <math.h>

using namespace daisysp;

void ZOscillator::Init(float sample_rate)
{
    sample_rate_ = sample_rate;

    carrier_phase_       = 0.0f;
    discontinuity_phase_ = 0.0f;
    formant_phase_       = 0.0f;
    next_sample_         = 0.0f;

    SetFreq(220.f);
    SetFormantFreq(550.f);
    SetMode(0.f);
    SetShape(1.f);
}

float ZOscillator::Process()
{
    float next_sample = next_sample_;

    bool  reset      = false;
    float reset_time = 0.0f;

    float this_sample = next_sample;
    next_sample       = 0.0f;

    discontinuity_phase_ += 2.0f * carrier_frequency_;
    carrier_phase_ += carrier_frequency_;
    reset = discontinuity_phase_ >= 1.0f;

    if(reset)
    {
        discontinuity_phase_ -= 1.0f;
        reset_time = discontinuity_phase_ / (2.0f * carrier_frequency_);

        float carrier_phase_before = carrier_phase_ >= 1.0f ? 1.0f : 0.5f;
        float carrier_phase_after  = carrier_phase_ >= 1.0f ? 0.0f : 0.5f;

        float mode_sub  = mode_ + (1.f - reset_time) * (mode_ - mode_new_);
        float shape_sub = carrier_shape_
                          + (1.0f - reset_time) * (carrier_shape_ - shape_new_);
        float before
            = Z(carrier_phase_before,
                1.0f,
                formant_phase_ + (1.0f - reset_time) * formant_frequency_,
                shape_sub,
                mode_sub);

        float after = Z(carrier_phase_after, 0.0f, 0.0f, shape_new_, mode_new_);

        float discontinuity = after - before;
        this_sample += discontinuity * ThisBlepSample(reset_time);
        next_sample += discontinuity * NextBlepSample(reset_time);
        formant_phase_ = reset_time * formant_frequency_;

        if(carrier_phase_ > 1.0f)
        {
            carrier_phase_ = discontinuity_phase_ * 0.5f;
        }
    }
    else
    {
        formant_phase_ += formant_frequency_;
        if(formant_phase_ >= 1.0f)
        {
            formant_phase_ -= 1.0f;
        }
    }

    if(carrier_phase_ >= 1.0f)
    {
        carrier_phase_ -= 1.0f;
    }

    carrier_shape_ = shape_new_;
    mode_          = mode_new_;
    next_sample += Z(carrier_phase_,
                     discontinuity_phase_,
                     formant_phase_,
                     carrier_shape_,
                     mode_);

    next_sample_ = next_sample;
    return this_sample;
}

inline float ZOscillator::Sine(float phase)
{
    return sinf(phase * TWOPI_F);
}

void ZOscillator::SetFreq(float freq)
{
    //convert from Hz to phase_inc / sample
    carrier_frequency_ = freq / sample_rate_;
    carrier_frequency_ = carrier_frequency_ >= .25f ? .25f : carrier_frequency_;
}

void ZOscillator::SetFormantFreq(float freq)
{
    //convert from Hz to phase_inc / sample
    formant_frequency_ = freq / sample_rate_;
    formant_frequency_ = formant_frequency_ >= .25f ? .25f : formant_frequency_;
}

void ZOscillator::SetShape(float shape)
{
    shape_new_ = shape;
}

void ZOscillator::SetMode(float mode)
{
    mode_new_ = mode;
}

inline float ZOscillator::Z(float c, float d, float f, float shape, float mode)
{
    float ramp_down = 0.5f * (1.0f + Sine(0.5f * d + 0.25f));

    float offset;
    float phase_shift;
    if(mode < 0.333f)
    {
        offset      = 1.0f;
        phase_shift = 0.25f + mode * 1.50f;
    }
    else if(mode < 0.666f)
    {
        phase_shift = 0.7495f - (mode - 0.33f) * 0.75f;
        offset      = -Sine(phase_shift);
    }
    else
    {
        phase_shift = 0.7495f - (mode - 0.33f) * 0.75f;
        offset      = 0.001f;
    }

    float discontinuity = Sine(f + phase_shift);
    float contour;
    if(shape < 0.5f)
    {
        shape *= 2.0f;
        if(c >= 0.5f)
        {
            ramp_down *= shape;
        }
        contour = 1.0f + (Sine(c + 0.25f) - 1.0f) * shape;
    }
    else
    {
        contour = Sine(c + shape * 0.5f);
    }
    return (ramp_down * (offset + discontinuity) - offset) * contour;
}