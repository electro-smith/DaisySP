#include "dsp.h"
#include "grainlet.h"
#include <math.h>

using namespace daisysp;

void GrainletOscillator::Init()
{
    carrier_phase_ = 0.0f;
    formant_phase_ = 0.0f;
    next_sample_   = 0.0f;

    carrier_frequency_ = 0.0f;
    formant_frequency_ = 0.0f;
    carrier_shape_     = 0.0f;
    carrier_bleed_     = 0.0f;
}

void GrainletOscillator::Render(float  carrier_frequency,
                                float  formant_frequency,
                                float  carrier_shape,
                                float  carrier_bleed,
                                float* out,
                                size_t size)
{
    float kMaxFrequency = .25f;

    if(carrier_frequency >= kMaxFrequency * 0.5f)
    {
        carrier_frequency = kMaxFrequency * 0.5f;
    }
    if(formant_frequency >= kMaxFrequency)
    {
        formant_frequency = kMaxFrequency;
    }

    stmlib::ParameterInterpolator carrier_frequency_modulation(
        &carrier_frequency_, carrier_frequency, size);
    stmlib::ParameterInterpolator formant_frequency_modulation(
        &formant_frequency_, formant_frequency, size);
    stmlib::ParameterInterpolator carrier_shape_modulation(
        &carrier_shape_, carrier_shape, size);
    stmlib::ParameterInterpolator carrier_bleed_modulation(
        &carrier_bleed_, carrier_bleed, size);

    float next_sample = next_sample_;

    while(size--)
    {
        bool  reset      = false;
        float reset_time = 0.0f;

        float this_sample = next_sample;
        next_sample       = 0.0f;

        const float f0 = carrier_frequency_modulation.Next();
        const float f1 = formant_frequency_modulation.Next();

        carrier_phase_ += f0;
        reset = carrier_phase_ >= 1.0f;

        if(reset)
        {
            carrier_phase_ -= 1.0f;
            reset_time   = carrier_phase_ / f0;
            float before = Grainlet(
                1.0f,
                formant_phase_ + (1.0f - reset_time) * f1,
                carrier_shape_modulation.subsample(1.0f - reset_time),
                carrier_bleed_modulation.subsample(1.0f - reset_time));

            float after = Grainlet(0.0f,
                                   0.0f,
                                   carrier_shape_modulation.subsample(1.0f),
                                   carrier_bleed_modulation.subsample(1.0f));

            float discontinuity = after - before;
            this_sample += discontinuity * ThisBlepSample(reset_time);
            next_sample += discontinuity * NextBlepSample(reset_time);
            formant_phase_ = reset_time * f1;
        }
        else
        {
            formant_phase_ += f1;
            if(formant_phase_ >= 1.0f)
            {
                formant_phase_ -= 1.0f;
            }
        }

        next_sample += Grainlet(carrier_phase_,
                                formant_phase_,
                                carrier_shape_modulation.Next(),
                                carrier_bleed_modulation.Next());
        *out++ = this_sample;
    }

    next_sample_ = next_sample;
}

float GrainletOscillator::Sine(float phase)
{
    return sinf(phase * TWOPI_F);
}

float GrainletOscillator::Carrier(float phase, float shape)
{
    shape *= 3.0f;
    int shape_integral = static_cast<int>(shape);
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