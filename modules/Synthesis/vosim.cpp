#include "dsp.h"
#include "vosim.h"
#include <math.h>

using namespace daisysp;

void VosimOscillator::Init(float sample_rate)
{
    sample_rate_ = sample_rate;

    carrier_phase_   = 0.0f;
    formant_1_phase_ = 0.0f;
    formant_2_phase_ = 0.0f;

    SetFreq(105.f);
    SetForm1Freq(1390.f);
    SetForm2Freq(817.f);
    SetShape(.5f);
}

float VosimOscillator::Process()
{
    carrier_phase_ += carrier_frequency_;
    if(carrier_phase_ >= 1.0f)
    {
        carrier_phase_ -= 1.0f;
        float reset_time = carrier_phase_ / carrier_frequency_;
        formant_1_phase_ = reset_time * formant_1_frequency_;
        formant_2_phase_ = reset_time * formant_2_frequency_;
    }
    else
    {
        formant_1_phase_ += formant_1_frequency_;
        if(formant_1_phase_ >= 1.0f)
        {
            formant_1_phase_ -= 1.0f;
        }
        formant_2_phase_ += formant_2_frequency_;
        if(formant_2_phase_ >= 1.0f)
        {
            formant_2_phase_ -= 1.0f;
        }
    }

    float carrier         = Sine(carrier_phase_ * 0.5f + 0.25f) + 1.0f;
    float reset_phase     = 0.75f - 0.25f * carrier_shape_;
    float reset_amplitude = Sine(reset_phase);
    float formant_0 = Sine(formant_1_phase_ + reset_phase) - reset_amplitude;
    float formant_1 = Sine(formant_2_phase_ + reset_phase) - reset_amplitude;
    return carrier * (formant_0 + formant_1) * 0.25f + reset_amplitude;
}

void VosimOscillator::SetFreq(float freq)
{
    carrier_frequency_ = freq / sample_rate_;
    carrier_frequency_ = carrier_frequency_ > .25f ? .25f : carrier_frequency_;
}

void VosimOscillator::SetForm1Freq(float freq)
{
    formant_1_frequency_ = freq / sample_rate_;
    formant_1_frequency_
        = formant_1_frequency_ > .25f ? .25f : formant_1_frequency_;
}

void VosimOscillator::SetForm2Freq(float freq)
{
    formant_2_frequency_ = freq / sample_rate_;
    formant_2_frequency_
        = formant_2_frequency_ > .25f ? .25f : formant_2_frequency_;
}

void VosimOscillator::SetShape(float shape)
{
    carrier_shape_ = shape;
}

float VosimOscillator::Sine(float phase)
{
    return sinf(TWOPI_F * phase);
}