#include "atone.h"
#include <math.h>
#include "dsp.h"

using namespace daisysp;

void ATone::Init(float sample_rate)
{
    prevout_     = 0.0f;
    freq_        = 1000.0f;
    c2_          = 0.5f;
    sample_rate_ = sample_rate;
}

float ATone::Process(float &in)
{
    float out;

    out      = c2_ * (prevout_ + in);
    prevout_ = out - in;

    return out;
}

void ATone::CalculateCoefficients()
{
    float b, c2;

    b   = 2.0f - cosf(TWOPI_F * freq_ / sample_rate_);
    c2  = b - sqrtf(b * b - 1.0f);
    c2_ = c2;
}
