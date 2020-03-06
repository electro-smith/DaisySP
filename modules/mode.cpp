//
// Mode
//
// This code has been extracted from the soundpipe opcode "mode".
// It has been modified to work as a Daisy module.
//
// Originally extracted from Csound.
//
// Original Author(s): Francois Blanc, Steven Yi
// Year: 2001
// Location: Opcodes/biquad.c
//

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#define ROOT2 (1.4142135623730950488f)

#ifndef M_PI
#define M_PI 3.14159265358979323846f /* pi */
#endif

#include "mode.h"

using namespace daisysp;

void Mode::Init(float sample_rate)
{
    freq_ = 500.0f;
    q_    = 50;

    xnm1_ = ynm1_ = ynm2_ = 0.0f;
    a0_ = a1_ = a2_ = d_ = 0.0f;
    lfq_ = lq_ = -1.0f;
    sr_        = sample_rate;
}

void Mode::Clear()
{
    xnm1_ = ynm1_ = ynm2_ = 0.0f;
    a0_ = a1_ = a2_ = 0.0f;
    d_              = 0.0f;
    lfq_            = -1.0f;
    lq_             = -1.0f;
}

float Mode::Process(float in)
{
    float out;
    float lfq = lfq_, lq = lq_;
    float xn, yn, a0 = a0_, a1 = a1_, a2 = a2_, d = d_;
    float xnm1 = xnm1_, ynm1 = ynm1_, ynm2 = ynm2_;
    float kfq = freq_;
    float kq  = q_;

    if(lfq != kfq || lq != kq)
    {
        float kfreq  = kfq * (2.0f * (float)M_PI);
        float kalpha = (sr_ / kfreq);
        float kbeta  = kalpha * kalpha;
        d            = 0.5f * kalpha;
        lq           = kq;
        lfq          = kfq;
        a0           = 1.0f / (kbeta + d / kfreq);
        a1           = a0 * (1.0f - 2.0f * kbeta);
        a2           = a0 * (kbeta - d / kq);
    }
    xn = in;

    yn = a0 * xnm1 - a1 * ynm1 - a2 * ynm2;

    xnm1 = xn;
    ynm2 = ynm1;
    ynm1 = yn;

    yn = yn * d;

    out   = yn;
    xnm1_ = xnm1;
    ynm1_ = ynm1;
    ynm2_ = ynm2;
    lfq_  = lfq;
    lq_   = lq;
    d_    = d;
    a0_   = a0;
    a1_   = a1;
    a2_   = a2;
    return out;
}
