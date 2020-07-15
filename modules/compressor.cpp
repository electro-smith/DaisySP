// # compressor
//
// Author: shensley
//

//#include <math.h>
#include <cmath>
#include <stdlib.h>
#include <stdint.h>
#include "compressor.h"

using namespace daisysp;

// From faust args are:
// 0 - atk
// 1 - ratio
// 2 - rel
// 3 - thresh

#ifndef max
#define max(a, b) ((a < b) ? b : a)
#endif

#ifndef min
#define min(a, b) ((a < b) ? a : b)
#endif


// This function and the two macros below come from:
// http://openaudio.blogspot.com/2017/02/faster-log10-and-pow.html
static float log2f_approx(float X)
{
    float Y, F;
    int   E;
    F = frexpf(fabsf(X), &E);
    Y = 1.23149591368684f;
    Y *= F;
    Y += -4.11852516267426f;
    Y *= F;
    Y += 6.02197014179219f;
    Y *= F;
    Y += -3.13396450166353f;
    Y += E;
    return (Y);
}

#define pow10f(x) expf(2.302585092994046f * x)
#define log10f_fast(x) (log2f_approx(x) * 0.3010299956639812f)

void Compressor::Init(float sample_rate)
{
    sr_       = sample_rate;
    i_const0_ = min(192000, max(1, sr_));
    f_const1_ = 2.0f / (float)i_const0_;
    f_const2_ = 1.0f / (float)i_const0_;
    // Skipped fHsliderN inits, but I'm going to init the 4 params
    ratio_  = 2.0f;
    thresh_ = -12.0f;
    atk_    = 0.1f;
    rel_    = 0.1f;
    for(uint8_t i = 0; i < 2; i++)
    {
        f_rec0_[i] = 0.1f;
        f_rec1_[i] = 0.1f;
        f_rec2_[i] = 0.1f;
    }
    RecalculateSlopes();
}

//float compressor::process(const float &in)
float Compressor::Process(float in, float key)
{
    float out;
    // Makeup gain may still be a little hot.
    float f_temp1 = fabsf(key);
    float f_temp2 = ((f_rec1_[1] > f_temp1) ? f_slow4_ : f_slow3_);
    f_rec2_[0]    = ((f_rec2_[1] * f_temp2) + ((1.0f - f_temp2) * f_temp1));
    f_rec1_[0]    = f_rec2_[0];
    f_rec0_[0]
        = ((f_slow1_ * f_rec0_[1])
           + (f_slow2_ * max(((20.f * log10f_fast(f_rec1_[0])) - f_slow5_), 0.f)));
//    f_rec0_[0]
//        = ((f_slow1_ * f_rec0_[1])
//           + (f_slow2_ * max(((20.f * log10f(f_rec1_[0])) - f_slow5_), 0.f)));
    //out = (powf(10.0f, (0.05f * f_rec0_[0] * (0.05f * makeup_gain_))) * in);
    out        = pow10f(0.05f * f_rec0_[0]) * in;
    f_rec2_[1] = f_rec2_[0];
    f_rec1_[1] = f_rec1_[0];
    f_rec0_[1] = f_rec0_[0];
    return out;
}

// Same without sidechain
float Compressor::Process(float in)
{
    return Process(in, in);
}

void Compressor::RecalculateSlopes()
{
    //makeup_gain_ = fabsf(thresh_ / ratio_) / 2.0f;
    makeup_gain_ = 1.0f;
    f_slow0_     = (float)atk_; // probably need to scale
    f_slow1_     = expf(0.0f - (f_const1_ / f_slow0_));
    f_slow2_     = ((1.0f - f_slow1_) * ((1.0f / ratio_) - 1.0f));
    f_slow3_     = expf((0.0f - (f_const2_ / f_slow0_)));
    f_slow4_     = expf((0.0f - (f_const2_ / rel_)));
    f_slow5_     = thresh_;
}
