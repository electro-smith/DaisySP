#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "pluck.h"

#define PLUKMIN 64
#define INTERPFACTOR 256.0f
#define INTERPFACTOR_I 255

using namespace daisysp;

void Pluck::Reinit()
{
    int    n;
    float  val = 0;
    float *ap  = buf_;
    //npts_ = (int32_t)roundf(decay_ * (float)(maxpts_ - PLUKMIN) + PLUKMIN);
    npts_ = (int32_t)(decay_ * (float)(maxpts_ - PLUKMIN) + PLUKMIN);
    //sicps_ = ((float)npts_ * INTERPFACTOR + INTERPFACTOR/2.0f) * (1.0f / _sr);
    sicps_ = ((float)npts_ * 256.0f + 128.0f) * (1.0f / sample_rate_);
    for(n = npts_; n--;)
    {
        val   = (float)((float)rand() / (float)RAND_MAX);
        *ap++ = (val * 2.0f) - 1.0f;
    }
    phs256_ = 0;
}

void Pluck::Init(float sample_rate, float *buf, int32_t npts, int32_t mode)
{
    amp_         = 0.5f;
    freq_        = 300;
    decay_       = 1.0f;
    sample_rate_ = sample_rate;
    mode_        = mode;

    maxpts_ = npts;
    npts_   = npts;
    buf_    = buf;

    Reinit();
    /* tuned pitch convt */
    sicps_ = (npts * 256.0f + 128.0f) * (1.0f / sample_rate_);
    init_  = 1;
}

float Pluck::Process(float &trig)
{
    float * fp, out;
    int32_t phs256, phsinc, ltwopi, offset;
    float   coeff;

    // unused variable
    // float inv_coeff;

    float frac, diff;
    float dampmin = 0.42f;

    if(trig != 0)
    {
        init_ = 0;
        Reinit();
    }

    if(init_)
    {
        return 0;
    }
    // Set Coeff for mode.
    switch(mode_)
    {
        case PLUCK_MODE_RECURSIVE:
            coeff = ((0.5f - dampmin) * damp_) + dampmin;
            break;
        case PLUCK_MODE_WEIGHTED_AVERAGE:
            coeff = 0.05f + (damp_ * 0.90f);
            break;
        default: coeff = 0.5f; break;
    }

    // variable set but not used
    //inv_coeff = 1.0f - coeff;

    phsinc = (int32_t)(freq_ * sicps_);
    phs256 = phs256_;
    ltwopi = npts_ << 8;
    offset = phs256 >> 8;
    fp     = (float *)buf_ + offset; /* lookup position   */
    diff   = fp[1] - fp[0];
    frac   = (float)(phs256 & 255) / 256.0f; /*  w. interpolation */
    out    = (fp[0] + diff * frac) * amp_;   /*  gives output val */
    if((phs256 += phsinc) >= ltwopi)
    {
        int   nn;
        float preval;
        phs256 -= ltwopi;
        fp     = buf_;
        preval = fp[0];
        fp[0]  = fp[npts_];
        fp++;
        nn = npts_;
        do
        {
            /* 1st order recursive filter*/
            //preval = (*fp + preval) * coeff;
            /* weighted average - stretches decay times */
            switch(mode_)
            {
                case PLUCK_MODE_RECURSIVE:
                    preval = (*fp + preval) * coeff;
                    break;
                case PLUCK_MODE_WEIGHTED_AVERAGE:
                    preval = (*fp * coeff) + (preval * (1.0f - coeff));
                    break;
                default: break;
            }
            *fp++ = preval;
        } while(--nn);
    }
    phs256_ = phs256;
    return out;
}
