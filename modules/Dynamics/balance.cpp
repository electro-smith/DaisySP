#include "balance.h"
#include <math.h>
#include "dsp.h"

using namespace daisysp;

void Balance::Init(float sample_rate)
{
    float b;
    sample_rate_ = sample_rate;
    ihp_         = 10.0f;
    b            = 2.0f - cosf(ihp_ * (TWOPI_F / sample_rate_));
    c2_          = b - sqrtf(b * b - 1.0f);
    c1_          = 1.0f - c2_;
    prvq_ = prvr_ = prva_ = 0.0f;
}

float Balance::Process(float sig, float comp)
{
    float q, r, a, diff, out;
    float c1 = c1_;
    float c2 = c2_;

    q        = prvq_;
    r        = prvr_;
    float as = sig;
    float cs = comp;

    q = c1 * as * as + c2 * q;
    r = c1 * cs * cs + c2 * r;

    prvq_ = q;
    prvr_ = r;

    if(q != 0.0f)
    {
        a = sqrtf(r / q);
    }
    else
    {
        a = sqrtf(r);
    }

    if((diff = a - prva_) != 0.0f)
    {
        out = sig * prva_;
    }
    else
    {
        out = sig * a;
    }

    prva_ = a;

    return out;
}
