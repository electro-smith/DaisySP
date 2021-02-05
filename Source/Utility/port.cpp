#include <math.h>
#include <stdlib.h>
#define ROOT2 (1.4142135623730950488)

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "port.h"

using namespace daisysp;

void Port::Init(float sample_rate, float htime)
{
    yt1_     = 0;
    prvhtim_ = -100.0;
    htime_   = htime;

    sample_rate_ = sample_rate;
    onedsr_      = 1.0 / sample_rate_;
}

float Port::Process(float in)
{
    if(prvhtim_ != htime_)
    {
        c2_      = pow(0.5, onedsr_ / htime_);
        c1_      = 1.0 - c2_;
        prvhtim_ = htime_;
    }

    return yt1_ = c1_ * in + c2_ * yt1_;
}
