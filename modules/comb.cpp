#include "comb.h"
#include <math.h>

using namespace daisysp;

static float log001 = -6.9078f; // log .001

void Comb::Init(float sample_rate)
{
    sample_rate_ = sample_rate;
    rev_time_  = 3.5;
    loop_time_ = 0.1;
    buf_size_  = (loop_time_ * sample_rate_); 
    prvt_ = 0.0f;
    coef_ = 0.0f;
    buf_pos_ = 0;
}

float Comb::Process(float in)
{
    float tmp = 0;
    float coef = coef_;
    float outsamp = 0;

    if ( prvt_ != rev_time_)
    {
        prvt_ = rev_time_;
	float exp_arg = (float) (log001 * loop_time_ / prvt_);
	if (exp_arg < -36.8413615)
	{
   	    coef = coef_ = 0;
	}
	else
	{
  	    coef = coef_ = expf(exp_arg);
	}
    }

    outsamp = buf_[buf_pos_];
    tmp = outsamp;
    tmp *= coef;
    tmp += in;
    buf_[buf_pos_] = tmp;

    buf_pos_++;
    buf_pos_ %= buf_size_;

    return outsamp;
}
