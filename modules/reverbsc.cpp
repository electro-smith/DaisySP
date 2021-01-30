#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "reverbsc.h"

#define REVSC_OK 0
#define REVSC_NOT_OK 1

#define DEFAULT_SRATE 48000.0
#define MIN_SRATE 5000.0
#define MAX_SRATE 1000000.0
#define MAX_PITCHMOD 20.0
#define DELAYPOS_SHIFT 28
#define DELAYPOS_SCALE 0x10000000
#define DELAYPOS_MASK 0x0FFFFFFF

#ifndef M_PI
#define M_PI 3.14159265358979323846 /* pi */
#endif

using namespace daisysp;

/* kReverbParams[n][0] = delay time (in seconds)                     */
/* kReverbParams[n][1] = random variation in delay time (in seconds) */
/* kReverbParams[n][2] = random variation frequency (in 1/sec)       */
/* kReverbParams[n][3] = random seed (0 - 32767)                     */

static const float kReverbParams[8][4]
    = {{(2473.0 / DEFAULT_SRATE), 0.0010, 3.100, 1966.0},
       {(2767.0 / DEFAULT_SRATE), 0.0011, 3.500, 29491.0},
       {(3217.0 / DEFAULT_SRATE), 0.0017, 1.110, 22937.0},
       {(3557.0 / DEFAULT_SRATE), 0.0006, 3.973, 9830.0},
       {(3907.0 / DEFAULT_SRATE), 0.0010, 2.341, 20643.0},
       {(4127.0 / DEFAULT_SRATE), 0.0011, 1.897, 22937.0},
       {(2143.0 / DEFAULT_SRATE), 0.0017, 0.891, 29491.0},
       {(1933.0 / DEFAULT_SRATE), 0.0006, 3.221, 14417.0}};

static int DelayLineMaxSamples(float sr, float i_pitch_mod, int n);
//static int InitDelayLine(dsy_reverbsc_dl *lp, int n);
static int         DelayLineBytesAlloc(float sr, float i_pitch_mod, int n);
static const float kOutputGain = 0.35;
static const float kJpScale    = 0.25;

int ReverbSc::Init(float sr)
{
    i_sample_rate_ = sr;
    sample_rate_   = sr;
    feedback_      = 0.97;
    lpfreq_        = 10000;
    i_pitch_mod_   = 1;
    i_skip_init_   = 0;
    damp_fact_     = 1.0;
    prv_lpfreq_    = 0.0;
    init_done_     = 1;
    int i, n_bytes = 0;
    n_bytes = 0;
    for(i = 0; i < 8; i++)
    {
        if(n_bytes > DSY_REVERBSC_MAX_SIZE)
            return 1;
        delay_lines_[i].buf = (aux_) + n_bytes;
        InitDelayLine(&delay_lines_[i], i);
        n_bytes += DelayLineBytesAlloc(sr, 1, i);
    }
    return 0;
}

static int DelayLineMaxSamples(float sr, float i_pitch_mod, int n)
{
    float max_del;

    max_del = kReverbParams[n][0];
    max_del += (kReverbParams[n][1] * (float)i_pitch_mod * 1.125);
    return (int)(max_del * sr + 16.5);
}

static int DelayLineBytesAlloc(float sr, float i_pitch_mod, int n)
{
    int n_bytes = 0;

    n_bytes += (DelayLineMaxSamples(sr, i_pitch_mod, n) * (int)sizeof(float));
    return n_bytes;
}

void ReverbSc::NextRandomLineseg(ReverbScDl *lp, int n)
{
    float prv_del, nxt_del, phs_inc_val;

    /* update random seed */
    if(lp->seed_val < 0)
        lp->seed_val += 0x10000;
    lp->seed_val = (lp->seed_val * 15625 + 1) & 0xFFFF;
    if(lp->seed_val >= 0x8000)
        lp->seed_val -= 0x10000;
    /* length of next segment in samples */
    lp->rand_line_cnt = (int)((sample_rate_ / kReverbParams[n][2]) + 0.5);
    prv_del           = (float)lp->write_pos;
    prv_del -= ((float)lp->read_pos
                + ((float)lp->read_pos_frac / (float)DELAYPOS_SCALE));
    while(prv_del < 0.0)
        prv_del += lp->buffer_size;
    prv_del = prv_del / sample_rate_; /* previous delay time in seconds */
    nxt_del = (float)lp->seed_val * kReverbParams[n][1] / 32768.0;
    /* next delay time in seconds */
    nxt_del = kReverbParams[n][0] + (nxt_del * (float)i_pitch_mod_);
    /* calculate phase increment per sample */
    phs_inc_val           = (prv_del - nxt_del) / (float)lp->rand_line_cnt;
    phs_inc_val           = phs_inc_val * sample_rate_ + 1.0;
    lp->read_pos_frac_inc = (int)(phs_inc_val * DELAYPOS_SCALE + 0.5);
}

int ReverbSc::InitDelayLine(ReverbScDl *lp, int n)
{
    float read_pos;
    /* int     i; */

    /* calculate length of delay line */
    lp->buffer_size = DelayLineMaxSamples(sample_rate_, 1, n);
    lp->dummy       = 0;
    lp->write_pos   = 0;
    /* set random seed */
    lp->seed_val = (int)(kReverbParams[n][3] + 0.5);
    /* set initial delay time */
    read_pos     = (float)lp->seed_val * kReverbParams[n][1] / 32768;
    read_pos     = kReverbParams[n][0] + (read_pos * (float)i_pitch_mod_);
    read_pos     = (float)lp->buffer_size - (read_pos * sample_rate_);
    lp->read_pos = (int)read_pos;
    read_pos     = (read_pos - (float)lp->read_pos) * (float)DELAYPOS_SCALE;
    lp->read_pos_frac = (int)(read_pos + 0.5);
    /* initialise first random line segment */
    NextRandomLineseg(lp, n);
    /* clear delay line to zero */
    lp->filter_state = 0.0;
    for(int i = 0; i < lp->buffer_size; i++)
    {
        lp->buf[i] = 0;
    }
    return REVSC_OK;
}

int ReverbSc::Process(const float &in1,
                      const float &in2,
                      float *      out1,
                      float *      out2)
{
    float       a_in_l, a_in_r, a_out_l, a_out_r;
    float       vm1, v0, v1, v2, am1, a0, a1, a2, frac;
    ReverbScDl *lp;
    int         read_pos;
    uint32_t    n;
    int         buffer_size; /* Local copy */
    float       damp_fact = damp_fact_;

    //if (init_done_ <= 0) return REVSC_NOT_OK;
    if(init_done_ <= 0)
        return REVSC_NOT_OK;

    /* calculate tone filter coefficient if frequency changed */
    if(lpfreq_ != prv_lpfreq_)
    {
        prv_lpfreq_ = lpfreq_;
        damp_fact
            = 2.0f - cosf(prv_lpfreq_ * (2.0f * (float)M_PI) / sample_rate_);
        damp_fact = damp_fact_
            = damp_fact - sqrtf(damp_fact * damp_fact - 1.0f);
    }

    /* calculate "resultant junction pressure" and mix to input signals */

    a_in_l = a_out_l = a_out_r = 0.0;
    for(n = 0; n < 8; n++)
    {
        a_in_l += delay_lines_[n].filter_state;
    }
    a_in_l *= kJpScale;
    a_in_r = a_in_l + in2;
    a_in_l = a_in_l + in1;

    /* loop through all delay lines */

    for(n = 0; n < 8; n++)
    {
        lp          = &delay_lines_[n];
        buffer_size = lp->buffer_size;

        /* send input signal and feedback to delay line */

        lp->buf[lp->write_pos]
            = (float)((n & 1 ? a_in_r : a_in_l) - lp->filter_state);
        if(++lp->write_pos >= buffer_size)
        {
            lp->write_pos -= buffer_size;
        }

        /* read from delay line with cubic interpolation */

        if(lp->read_pos_frac >= DELAYPOS_SCALE)
        {
            lp->read_pos += (lp->read_pos_frac >> DELAYPOS_SHIFT);
            lp->read_pos_frac &= DELAYPOS_MASK;
        }
        if(lp->read_pos >= buffer_size)
            lp->read_pos -= buffer_size;
        read_pos = lp->read_pos;
        frac     = (float)lp->read_pos_frac * (1.0 / (float)DELAYPOS_SCALE);

        /* calculate interpolation coefficients */

        a2 = frac * frac;
        a2 -= 1.0;
        a2 *= (1.0 / 6.0);
        a1 = frac;
        a1 += 1.0;
        a1 *= 0.5;
        am1 = a1 - 1.0;
        a0  = 3.0 * a2;
        a1 -= a0;
        am1 -= a2;
        a0 -= frac;

        /* read four samples for interpolation */

        if(read_pos > 0 && read_pos < (buffer_size - 2))
        {
            vm1 = (float)(lp->buf[read_pos - 1]);
            v0  = (float)(lp->buf[read_pos]);
            v1  = (float)(lp->buf[read_pos + 1]);
            v2  = (float)(lp->buf[read_pos + 2]);
        }
        else
        {
            /* at buffer wrap-around, need to check index */

            if(--read_pos < 0)
                read_pos += buffer_size;
            vm1 = (float)lp->buf[read_pos];
            if(++read_pos >= buffer_size)
                read_pos -= buffer_size;
            v0 = (float)lp->buf[read_pos];
            if(++read_pos >= buffer_size)
                read_pos -= buffer_size;
            v1 = (float)lp->buf[read_pos];
            if(++read_pos >= buffer_size)
                read_pos -= buffer_size;
            v2 = (float)lp->buf[read_pos];
        }
        v0 = (am1 * vm1 + a0 * v0 + a1 * v1 + a2 * v2) * frac + v0;

        /* update buffer read position */

        lp->read_pos_frac += lp->read_pos_frac_inc;

        /* apply feedback gain and lowpass filter */

        v0 *= (float)feedback_;
        v0               = (lp->filter_state - v0) * damp_fact + v0;
        lp->filter_state = v0;

        /* mix to output */

        if(n & 1)
        {
            a_out_r += v0;
        }
        else
        {
            a_out_l += v0;
        }

        /* start next random line segment if current one has reached endpoint */

        if(--(lp->rand_line_cnt) <= 0)
        {
            NextRandomLineseg(lp, n);
        }
    }
    /* someday, use a_out_r for multimono out */

    *out1 = a_out_l * kOutputGain;
    *out2 = a_out_r * kOutputGain;
    return REVSC_OK;
}
