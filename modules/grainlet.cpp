#include "dsp.h"
#include "grainlet.h"
#include <math.h>

using namespace daisysp;

void GrainletOscillator::Init(float sample_rate)
{
    sample_rate_ = sample_rate;

    carrier_phase_ = 0.0f;
    formant_phase_ = 0.0f;
    next_sample_   = 0.0f;

    carrier_shape_ = 0.f;
    carrier_bleed_ = 0.f;

    SetFreq(440.f);
    SetFormantFreq(220.f);
    SetShape(.5f);
    SetBleed(.5f);
}

float GrainletOscillator::Process()
{
    float this_sample = next_sample_;
    float next_sample = 0.0f;

    carrier_phase_ += carrier_frequency_;

    if(carrier_phase_ >= 1.0f)
    {
        carrier_phase_ -= 1.0f;
        float reset_time = carrier_phase_ / carrier_frequency_;

        float shape_inc = new_carrier_shape_ - carrier_shape_;
        float bleed_inc = new_carrier_bleed_ - carrier_bleed_;

        float before = Grainlet(
            1.0f,
            formant_phase_ + (1.0f - reset_time) * formant_frequency_,
            new_carrier_shape_ + shape_inc * (1.0f - reset_time),
            new_carrier_bleed_ + bleed_inc * (1.0f - reset_time));

        float after
            = Grainlet(0.0f, 0.0f, new_carrier_shape_, new_carrier_bleed_);

        float discontinuity = after - before;
        this_sample += discontinuity * ThisBlepSample(reset_time);
        next_sample += discontinuity * NextBlepSample(reset_time);
        formant_phase_ = reset_time * formant_frequency_;
    }
    else
    {
        formant_phase_ += formant_frequency_;
        if(formant_phase_ >= 1.0f)
        {
            formant_phase_ -= 1.0f;
        }
    }

    carrier_bleed_ = new_carrier_bleed_;
    carrier_shape_ = new_carrier_shape_;
    next_sample += Grainlet(
        carrier_phase_, formant_phase_, carrier_shape_, carrier_bleed_);
    next_sample_ = next_sample;
    return this_sample;
}

void GrainletOscillator::SetFreq(float freq)
{
    carrier_frequency_ = freq / sample_rate_;
    carrier_frequency_ = carrier_frequency_ > 0.5f ? 0.5f : carrier_frequency_;
}

void GrainletOscillator::SetFormantFreq(float freq)
{
    formant_frequency_ = freq / sample_rate_;
    formant_frequency_ = formant_frequency_ > 0.5f ? 0.5f : formant_frequency_;
}

void GrainletOscillator::SetShape(float shape)
{
    new_carrier_shape_ = shape;
}

void GrainletOscillator::SetBleed(float bleed)
{
    new_carrier_bleed_ = bleed;
}


float GrainletOscillator::Sine(float phase)
{
    return sinf(phase * TWOPI_F);
}

float GrainletOscillator::Carrier(float phase, float shape)
{
    shape *= 3.0f;
    int   shape_integral   = static_cast<int>(shape);
    float shape_fractional = shape - static_cast<float>(shape_integral);

    float t = 1.0f - shape_fractional;

    if(shape_integral == 0)
    {
        phase = phase * (1.0f + t * t * t * 15.0f);
        if(phase >= 1.0f)
        {
            phase = 1.0f;
        }
        phase += 0.75f;
    }
    else if(shape_integral == 1)
    {
        float breakpoint = 0.001f + 0.499f * t * t * t;
        if(phase < breakpoint)
        {
            phase *= (0.5f / breakpoint);
        }
        else
        {
            phase = 0.5f + (phase - breakpoint) * 0.5f / (1.0f - breakpoint);
        }
        phase += 0.75f;
    }
    else
    {
        t     = 1.0f - t;
        phase = 0.25f + phase * (0.5f + t * t * t * 14.5f);
        if(phase >= 0.75f)
            phase = 0.75f;
    }
    return (Sine(phase) + 1.0f) * 0.25f;
}

float GrainletOscillator::Grainlet(float carrier_phase,
                                   float formant_phase,
                                   float shape,
                                   float bleed)
{
    float carrier = Carrier(carrier_phase, shape);
    float formant = Sine(formant_phase);
    return carrier * (formant + bleed) / (1.0f + bleed);
}