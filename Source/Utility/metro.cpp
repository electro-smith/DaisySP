#include <math.h>
#include "metro.h"
#include "dsp.h"

using namespace daisysp;

void Metro::Init(float freq, float sample_rate)
{
    freq_        = freq;
    phs_         = 0.0f;
    sample_rate_ = sample_rate;
    phs_inc_     = (TWOPI_F * freq_) / sample_rate_;
}

uint8_t Metro::Process()
{
    phs_ += phs_inc_;
    if(phs_ >= TWOPI_F)
    {
        phs_ -= TWOPI_F;
        return 1;
    }
    return 0;
}

void Metro::SetFreq(float freq)
{
    freq_    = freq;
    phs_inc_ = (TWOPI_F * freq_) / sample_rate_;
}
