#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include "jitter.h"

#ifndef FT_MAXLEN
#define FT_MAXLEN 0x1000000L
#endif

#ifndef FT_PHMASK
#define FT_PHMASK 0x0FFFFFFL
#endif

using namespace daisysp;


float Jitter::randGab()
{
    return (float)((rand() >> 1) & 0x7fffffff)
           * (4.656612875245796924105750827168e-10);
}

float Jitter::biRandGab()
{
    return (float)(rand() & 0x7fffffff)
           * (4.656612875245796924105750827168e-10);
}

void Jitter::SetAmp(float amp)
{
    amp_ = amp;
    Reset();
}

void Jitter::SetCpsMin(float cps_min)
{
    cps_min_ = cps_min;
    Reset();
}

void Jitter::SetCpsMax(float cps_max)
{
    cps_max_ = cps_max;
    Reset();
}

void Jitter::Reset()
{
    num2_      = biRandGab();
    init_flag_ = true;
    phs_       = 0;
}

void Jitter::Init(float sample_rate)
{
    sample_rate_ = sample_rate;
    amp_         = 0.5;
    cps_min_     = 0.5;
    cps_max_     = 4;
    Reset();
}

float Jitter::Process()
{
    float out;
    if(init_flag_)
    {
        init_flag_ = false;
        out        = num2_ * amp_;
        cps_       = randGab() * (cps_max_ - cps_min_) + cps_min_;
        phs_ &= FT_PHMASK;
        num1_    = num2_;
        num2_    = biRandGab();
        dfd_max_ = 1.0 * (num2_ - num1_) / FT_MAXLEN;
        return out;
    }

    out = (num1_ + (float)phs_ * dfd_max_) * amp_;
    phs_ += (int32_t)(cps_ * (float)(FT_MAXLEN / sample_rate_));

    if(phs_ >= FT_MAXLEN)
    {
        cps_ = randGab() * (cps_max_ - cps_min_) + cps_min_;
        phs_ &= FT_PHMASK;
        num1_    = num2_;
        num2_    = biRandGab();
        dfd_max_ = 1.0 * (num2_ - num1_) / FT_MAXLEN;
    }

    return out;
}
