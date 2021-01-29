#include "line.h"

using namespace daisysp;

void Line::Init(float sample_rate)
{
    sample_rate_ = sample_rate;
    dur_         = 0.5f;
    end_         = 0.0f;
    start_       = 1.0f;
    val_         = 1.0f;
}

void Line::Start(float start, float end, float dur)
{
    start_    = start;
    end_      = end;
    dur_      = dur;
    inc_      = (end - start) / ((sample_rate_ * dur_));
    val_      = start_;
    finished_ = 0;
}

float Line::Process(uint8_t *finished)
{
    float out;
    out = val_;

    if((end_ > start_ && out >= end_) || (end_ < start_ && out <= end_))
    {
        finished_ = 1;
        val_      = end_;
        out       = end_;
    }
    else
    {
        val_ += inc_;
    }
    *finished = finished_;
    return out;
}
