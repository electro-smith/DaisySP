#include <math.h>
#include "dcblock.h"

using namespace daisysp;

void DcBlock::Init(float sample_rate)
{
    output_ = 0.0;
    input_  = 0.0;
    gain_   = expf(-2.0f * M_PI * 10.f / sample_rate);
}

float DcBlock::Process(float in)
{
    float out;
    out     = in - input_ + (gain_ * output_);
    output_ = out;
    input_  = in;
    return out;
}
