#include "allpass.h"
#include <math.h>

using namespace daisysp;

void Allpass::Init(float sample_rate)
{
    sample_rate_ = sample_rate;
    rev_time_ = 3.5;
    loop_time_ = looptime;
    buf_size_ = 0.5 + loop_time_ * sample_rate_;
    //auxdata
    prvt_ = 0.0;
    coef_ = 0.0;
    buff_pos_ = 0;
}

float Allpass::Process(float in)
{
  float y, z, out;
    float coef = coef_;
    if (prvt_ != rev_time_)
    {
        prvt_ = rev_time_;
	coef = coef_ = expf(-6.9078 * loop_time_ / prvt_);
    }

    y = buf[bufpos_];
    out = y - coef_ * z;

    bufpos_++;
    bufpos %= buf_size_;
    return out;
}
