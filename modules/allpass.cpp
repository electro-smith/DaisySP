#include "allpass.h"
#include <math.h>

using namespace daisysp;

template <typename T, size_t size>
void Allpass<T, size>::Init(float sample_rate, Buffer <T, size> buf)
{
    sample_rate_ = sample_rate;
    rev_time_ = 3.5;
    max_loop_time_ = ((float) buf.size_ / sample_rate_) - .01;
    loop_time_ = max_loop_time_;
    mod_ = (int)(loop_time_ * sample_rate_);
    buf_ = buf;
    prvt_ = 0.0;
    coef_ = 0.0;
    buf_pos_ = 0;
}

template <typename T, size_t size>
float Allpass<T, size>::Process(float in)
{
    float y, z, out;
    if (prvt_ != rev_time_)
    {
        prvt_ = rev_time_;
	coef_ = expf(-6.9078 * loop_time_ / prvt_);
    }

    y = buf_.buff[buf_pos_];
    z = coef_ * y + in;
    buf_.buff[buf_pos_] = z;
    out = y - coef_ * z;

    buf_pos_++;
    buf_pos_ %= mod_;
    return out;
}

template <typename T, size_t size>
void Allpass<T, size>::SetFreq(float freq)
{
    loop_time_ = fminf(freq, max_loop_time_);
    mod_ = loop_time_ * sample_rate_;
}
