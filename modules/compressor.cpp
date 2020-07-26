// # compressor
//
// Author: shensley, AvAars
//

//#include <math.h>
#include <cmath>
#include <stdlib.h>
#include <stdint.h>
#include "compressor.h"
#include "dsp.h"

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

void Compressor::Init(float sample_rate)
{
    sample_rate_ = min(192000.0f, max(1.0f, sample_rate));
    sample_rate_inv_ = 1.0f / sample_rate_;
    sample_rate_inv2 = 2.0f / sample_rate_;
    // Skipped fHsliderN inits, but I'm going to init the 4 params
    ratio_       = 2.0f;
    thresh_      = -12.0f;
    atk_         = 0.1f;
    rel_         = 0.1f;
    makeup_mul_ = 1.0f;
    for(uint8_t i = 0; i < 2; i++)
    {
        f_rec0_[i] = 0.1f;
        f_rec1_[i] = 0.1f;
        f_rec2_[i] = 0.1f;
    }
    RecalculateSlopes();
}

float Compressor::Process(float in)
{
    float inAbs = fabsf(in);
    float f_temp2 = ((f_rec1_[1] > inAbs) ? rel_exp_ : atk_exp_);
    f_rec2_[0]    = ((f_rec2_[1] * f_temp2) + ((1.0f - f_temp2) * inAbs));
    f_rec1_[0]    = f_rec2_[0];
    f_rec0_[0]
        = ((atk_exp2_ * f_rec0_[1])
           + (ratio_mul_
              * fmax(((20.f * log10(f_rec1_[0])) - thresh_), 0.f)));
    gain_      = powf(10.0f, (0.05f * (f_rec0_[0] + makeup_gain_)));

    f_rec2_[1] = f_rec2_[0];
    f_rec1_[1] = f_rec1_[0];
    f_rec0_[1] = f_rec0_[0];
    return gain_ * in;
}

void Compressor::ProcessBlock(float *in, float *out, float *key, size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        Process(key[i]);
        out[i] = GetGain(in[i]);
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
            out[c][i] = GetGain(in[c][i]);
        }
    }
}

void Compressor::RecalculateSlopes()
{
    makeup_gain_ = fabsf(thresh_ - thresh_ / ratio_) * 0.5f * makeup_mul_;

    atk_exp2_ = expf(-(sample_rate_inv2 / atk_));
    ratio_mul_ = ((1.0f - atk_exp2_) * ((1.0f / ratio_) - 1.0f));

    atk_exp_ = expf((-(sample_rate_inv_ / atk_)));
    rel_exp_ = expf(( - (sample_rate_inv_ / rel_)));
}
