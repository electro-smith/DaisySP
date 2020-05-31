#include "decimator.h"

using namespace daisysp;

void Decimator::Init()
{
    downsample_factor_ = 1.0f;
    bitcrush_factor_   = 0.0f;
    downsampled_       = 0.0f;
    bitcrushed_        = 0.0f;
    inc_               = 0;
    threshold_         = 0;
}

float Decimator::Process(float input)
{
    int32_t temp;
    //downsample
    threshold_ = (uint32_t)((downsample_factor_ * downsample_factor_) * 96.0f);
    inc_ += 1;
    if(inc_ > threshold_)
    {
        inc_         = 0;
        downsampled_ = input;
    }
    //bitcrush
    temp = (int32_t)(downsampled_ * 65536.0f);
    temp >>= bits_to_crush_; // shift off
    temp <<= bits_to_crush_; // move back with zeros
    bitcrushed_ = (float)temp / 65536.0f;
    return bitcrushed_;
}
