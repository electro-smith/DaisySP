#include "fold.h"
#include <math.h>

using namespace daisysp;


void Fold::Init()
{
    incr_         = 1000.f;
    sample_index_ = 0;
    index_        = 0.0f;
    value_        = 0.0f;
}

float Fold::Process(float in)
{
    float out;

    if(index_ < sample_index_)
    {
        index_ += incr_;
        out = value_ = in;
    }
    else
    {
        out = value_;
    }

    sample_index_++;
    return out;
}
