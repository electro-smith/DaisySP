#include "bitcrush.h"
#include "fold.h"
#include <math.h>

using namespace daisysp;

static Fold fold;

void Bitcrush::Init(float sample_rate)
{
    bit_depth_   = 8;
    crush_rate_  = 10000;
    sample_rate_ = sample_rate;
    fold.Init();
}

float Bitcrush::Process(float in)
{
    float bits    = pow(2, bit_depth_);
    float foldamt = sample_rate_ / crush_rate_;
    float out;

    out = in * 65536.0f;
    out += 32768;
    out *= (bits / 65536.0f);
    out = floor(out);
    out *= (65536.0f / bits) - 32768;

    fold.SetIncrement(foldamt);
    out = fold.Process(out);
    out /= 65536.0;

    return out;
}
