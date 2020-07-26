// # compressor
//
// Author: shensley, AvAars
//

#include <cmath>
#include <stdlib.h>
#include <stdint.h>
#include "compressor.h"

using namespace daisysp;

#ifndef max
#define max(a, b) ((a < b) ? b : a)
#endif

#ifndef min
#define min(a, b) ((a < b) ? a : b)
#endif

void Compressor::Init(float sample_rate)
{
    sample_rate_      = min(192000, max(1, sample_rate));
    sample_rate_inv_  = 1.0f / (float)sample_rate_;
    sample_rate_inv2_ = 2.0f / (float)sample_rate_;

    // Initializing the params in this order to avoid dividing by zero

    SetRatio(2.0f);
    SetAttack(0.1f);
    SetRelease(0.1f);
    SetThreshold(-12.0f);
    AutoMakeup(true);

    gain_rec_  = 0.1f;
    slope_rec_ = 0.1f;
}

float Compressor::Process(float in)
{
    float inAbs   = fabsf(in);
    float cur_slo = ((slope_rec_ > inAbs) ? rel_slo_ : atk_slo_);
    slope_rec_    = ((slope_rec_ * cur_slo) + ((1.0f - cur_slo) * inAbs));
    gain_rec_     = ((atk_slo2_ * gain_rec_)
                 + (ratio_mul_
                    * fmax(((20.f * fastlog10f(slope_rec_)) - thresh_), 0.f)));
    gain_         = pow10f(0.05f * (gain_rec_ + makeup_gain_));

    return gain_ * in;
}

void Compressor::ProcessBlock(float *in, float *out, float *key, size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        Process(key[i]);
        out[i] = Apply(in[i]);
    }
}

// Multi-channel block processing
void Compressor::ProcessBlock(float **in,
                              float **out,
                              float * key,
                              size_t  channels,
                              size_t  size)
{
    for(size_t i = 0; i < size; i++)
    {
        Process(key[i]);
        for(size_t c = 0; c < channels; c++)
        {
            out[c][i] = Apply(in[c][i]);
        }
    }
}
