#include "allpass.h"
#include <math.h>

using namespace daisysp;

void Allpass::Init(float sample_rate, float* buff, size_t size)
{
    sample_rate_   = sample_rate;
    rev_time_      = 3.5;
    max_loop_time_ = ((float)size / sample_rate_) - .01;
    loop_time_     = max_loop_time_;
    mod_           = (int)(loop_time_ * sample_rate_);
    buf_           = buff;
    prvt_          = 0.0;
    coef_          = 0.0;
    buf_pos_       = 0;
}

float Allpass::Process(float in)
{
    float y, z, out;
    if(prvt_ != rev_time_)
    {
        prvt_ = rev_time_;
        coef_ = expf(-6.9078 * loop_time_ / prvt_);
    }

    y              = buf_[buf_pos_];
    z              = coef_ * y + in;
    buf_[buf_pos_] = z;
    out            = y - coef_ * z;

    buf_pos_++;
    buf_pos_ %= mod_;
    return out;
}

void Allpass::SetFreq(float freq)
{
    loop_time_ = fmaxf(fminf(freq, max_loop_time_), .0001);
    mod_       = fmaxf(loop_time_ * sample_rate_, 0);
}
