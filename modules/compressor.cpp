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
    sample_rate_     = min(192000, max(1, sample_rate));
    sample_rate_inv_ = 1.0f / (float)sample_rate_;
    sample_rate_inv2 = 2.0f / (float)sample_rate_;
    
	// Initializing the params in this order to avoid dividing by zero

    SetRatio(2.0f);
	SetAttack(0.1f);
    SetRelease(0.1f);
    SetThreshold(-12.0f);
    AutoMakeup(true);

    for(uint8_t i = 0; i < 2; i++)
    {
        gain_rec_[i]  = 0.1f;
        slope_rec_[i] = 0.1f;
    }
}

float Compressor::Process(float in)
{
    float inAbs   = fabsf(in);
    float cur_slo = ((slope_rec_[1] > inAbs) ? rel_slo_ : atk_slo_);
    slope_rec_[0] = ((slope_rec_[1] * cur_slo) + ((1.0f - cur_slo) * inAbs));
    gain_rec_[0]  = ((atk_slo2_ * gain_rec_[1])
                    + (ratio_mul_
                       * fmax(((20.f * log10(slope_rec_[0])) - thresh_), 0.f)));
    gain_         = powf(10.0f, (0.05f * (gain_rec_[0] + makeup_gain_)));

    slope_rec_[1] = slope_rec_[0];
    gain_rec_[1]  = gain_rec_[0];
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
