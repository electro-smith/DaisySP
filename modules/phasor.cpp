#include <math.h>
#include "phasor.h"

using namespace daisysp;

constexpr float TWO_PI_F = (float)(M_PI * 2.0);

void Phasor::SetFreq(float freq)
{
    freq_ = freq;
    inc_  = (TWO_PI_F * freq_) / sample_rate_;
}

float Phasor::Process()
{
    float out;
    out = phs_ / TWO_PI_F;
    phs_ += inc_;
    if(phs_ > TWO_PI_F)
    {
        phs_ -= TWO_PI_F;
    }
    if(phs_ < 0.0f)
    {
        phs_ = 0.0f;
    }
    return out;
}
