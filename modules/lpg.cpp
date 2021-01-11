#include "dsp.h"
#include "lpg.h"
#include <math.h>

using namespace daisysp;

void LowPassGate::Init(float sample_rate)
{
    filter_.Init(sample_rate);
    sample_rate_ = sample_rate;
}

float LowPassGate::Process(float gain,
                           float frequency,
                           float hf_bleed,
                           float in)
{
    //filter_.set_f_q<stmlib::FREQUENCY_DIRTY>(frequency, 0.4f);
    filter_.SetFreq(frequency);
    const float s = in * gain;
    filter_.Process(s);
    const float lp = filter_.Low();
    return lp + (s - lp) * hf_bleed;
}

float LowPassGate::Process(float  gain,
                           float  frequency,
                           float  hf_bleed,
                           size_t stride,
                           float  in)
{
    filter_.SetFreq(frequency);
    //filter_.set_f_q<stmlib::FREQUENCY_DIRTY>(frequency, 0.4f);
    const float s = in * gain;
    filter_.Process(s);
    const float lp = filter_.Low();
    return stride + Clip16(1 + static_cast<int32_t>(lp + (s - lp) * hf_bleed));
}