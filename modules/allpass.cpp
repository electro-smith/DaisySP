#include "allpass.h"
#include <math.h>

using namespace daisysp;

void Allpass::Init(float sample_rate)
{
    sample_rate_ = sample_rate;
    rev_time_ = 3.5;
    loop_time_ = 0.01f; //maximum of .5 seconds at 192kHz (96000 samples)
    buf_size_ = (int)(loop_time_ * sample_rate_);
    prvt_ = 0.0;
    coef_ = 0.0;
    buf_pos_ = 0;
}

float Allpass::Process(float in)
{
    float y, z, out;
    if (prvt_ != rev_time_)
    {
        prvt_ = rev_time_;
	coef_ = expf(-6.9078 * loop_time_ / prvt_);
    }

    y = buf_[buf_pos_];
    z = coef_ * y + in;
    buf_[buf_pos_] = z;
    out = y - coef_ * z;

    buf_pos_++;
    buf_pos_ %= buf_size_;
    return out;
}
