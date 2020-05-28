#include "comb.h"
#include <math.h>

using namespace daisysp;

static float log001 = -6.9078f; // log .001

template <typename T, size_t size>
void Comb<T, size>::Init(float sample_rate, Buffer <T, size> buf)
{
    sample_rate_ = sample_rate;
    rev_time_  = 3.5;
    max_loop_time_ = ((float) buf.size_ / sample_rate_) - .01;
    loop_time_ = max_loop_time_;
    mod_ = sample_rate_ * loop_time_;
    buf_ = buf;
    prvt_ = 0.0f;
    coef_ = 0.0f;
    buf_pos_ = 0;
}

template <typename T, size_t size>
float Comb<T, size>::Process(float in)
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

    outsamp = buf_.buff[buf_pos_];
    tmp = outsamp;
    tmp *= coef;
    tmp += in;
    buf_.buff[buf_pos_] = tmp;

    buf_pos_++;
    buf_pos_ %= mod_;

    return outsamp;
}

template <typename T, size_t size>
void Comb<T, size>::SetFreq(float freq)
{
    loop_time_ = fminf(freq, max_loop_time_);
    mod_ = loop_time_ * sample_rate_;
}
