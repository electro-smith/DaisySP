#include "tone.h"
#include <math.h>
#include "dsp.h"

using namespace daisysp;

void Tone::Init(float sample_rate)
{
    prevout_     = 0.0f;
    freq_        = 100.0f;
    c1_          = 0.5f;
    c2_          = 0.5f;
    sample_rate_ = sample_rate;
}

float Tone::Process(float &in)
{
    float out;

    out      = c1_ * in + c2_ * prevout_;
    prevout_ = out;

    return out;
}

void Tone::CalculateCoefficients()
{
    float b, c1, c2;
    b   = 2.0f - cosf(TWOPI_F * freq_ / sample_rate_);
    c2  = b - sqrtf(b * b - 1.0f);
    c1  = 1.0f - c2;
    c1_ = c1;
    c2_ = c2;
}
