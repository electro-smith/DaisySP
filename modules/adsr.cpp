#include "adsr.h"
#include <math.h>

using namespace daisysp;

float Adsr::Tau2Pole(float tau)
{
    return exp(-1.0 / (tau * sample_rate_));
}

float Adsr::AdsrFilter()
{
    y_ = b_ * x_ + a_ * y_;
    return y_;
}



void Adsr::Init(float sample_rate)
{
    seg_time_[ADSR_SEG_ATTACK]  = 0.1;
    seg_time_[ADSR_SEG_DECAY]   = 0.1;
    sus_                        = 0.5;
    seg_time_[ADSR_SEG_RELEASE] = 0.3;
    //timer_ = 0;
    a_ = 0;
    b_ = 0;
    x_ = 0;
    y_ = 0;
    prev_ = 0;
    atk_time_ = seg_time_[ADSR_SEG_ATTACK] * sample_rate;
    sample_rate_ = sample_rate;
}

float Adsr::Process(bool gate)
{
    float pole, out;
    out = 0.0;
    
    if (gate && mode_ != ADSR_SEG_DECAY)
    {
        mode_ = ADSR_SEG_ATTACK;
	//timer_ = 0;
	pole = Tau2Pole(seg_time_[ADSR_SEG_ATTACK] * 0.6);
	atk_time_ = seg_time_[ADSR_SEG_ATTACK] * sample_rate_;
	a_ = pole;
	b_ = 1.0 - pole;
    }
    else if(!gate)
    {
        mode_ = ADSR_SEG_RELEASE;
	pole = Tau2Pole(seg_time_[ADSR_SEG_RELEASE]);
    }
    
    x_ = (int)gate;
    prev_ = (int)gate;
    
    switch(mode_)
    {
        case ADSR_SEG_IDLE:
	    out = 0;
	    break;
        case ADSR_SEG_ATTACK:
	    out = AdsrFilter();

	    if (out > .99)
	    {
	        mode_ = ADSR_SEG_DECAY;
		pole = Tau2Pole(seg_time_[ADSR_SEG_DECAY]);
		a_ = pole;
		b_ = 1.0 - pole;
	    }
	    break;
        case ADSR_SEG_DECAY:
        case ADSR_SEG_RELEASE:
	    x_ *= sus_;
	    out = AdsrFilter();
        default:
	    break;
    }
    return out;
}
