#include "adsr.h"
#include <math.h>

using namespace daisysp;

float Adsr::Tau2Pole(float tau)
{
    return expf(-1.0f / (tau * sample_rate_));
}

float Adsr::AdsrFilter()
{
    y_ = b_ * x_ + a_ * y_;
    return y_;
}


void Adsr::Init(float sample_rate, int blockSize)
{
    sus_                 = 0.7f;
    SetTime(ADSR_SEG_ATTACK,  0.1f);
    SetTime(ADSR_SEG_DECAY,   0.1f);
    SetTime(ADSR_SEG_RELEASE, 0.1f);
    a_           = 0.0f;
    b_           = 0.0f;
    x_           = 0.0f;
    y_           = 0.0f;
    sample_rate_ = sample_rate / blockSize;
}

void
Adsr::SetTime(int seg, float time)
{
    if(seg_time_[seg] != time)
    {
        seg_time_[seg] = time;
        float target = (seg == ADSR_SEG_ATTACK)? 0.5 : (1./M_E);
        seg_D0_  [seg] = expf(logf(target) / (time * sample_rate_));
    }
}


float Adsr::Process(bool gate)
{
    float pole, out;
    out = 0.0f;

    if(gate && mode_ != ADSR_SEG_DECAY)
    {
        mode_ = ADSR_SEG_ATTACK;
        //timer_ = 0;
        pole      = Tau2Pole(seg_time_[ADSR_SEG_ATTACK] * 0.6f);
        a_        = pole;
        b_        = 1.0f - pole;
    }
    else if(!gate && mode_ != ADSR_SEG_IDLE)
    {
        mode_ = ADSR_SEG_RELEASE;
        pole  = Tau2Pole(seg_time_[ADSR_SEG_RELEASE]);
        a_    = pole;
        b_    = 1.0f - pole;
    }

    x_    = (int)gate;

    switch(mode_)
    {
        case ADSR_SEG_IDLE: out = 0.0f; break;
        case ADSR_SEG_ATTACK:
            out = AdsrFilter();

            if(out > .99f)
            {
                mode_ = ADSR_SEG_DECAY;
                pole  = Tau2Pole(seg_time_[ADSR_SEG_DECAY]);
                a_    = pole;
                b_    = 1.0f - pole;
            }
            break;
        case ADSR_SEG_DECAY:
        case ADSR_SEG_RELEASE:
            x_ *= sus_;
            out = AdsrFilter();
            if(out <= 0.01f)
                mode_ = ADSR_SEG_IDLE;
        default: break;
    }
    return out;
}
