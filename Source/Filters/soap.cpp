#include "soap.h"
#include <cmath>

#define PI 3.141592653589793

using namespace daisysp;

void Soap::Init(float sample_rate)
{
    soap_center_freq_ = 400.0;
    soap_bandwidth_   = 50.0;
    in_0_             = 0.0; // input            x0
    din_1_            = 0.0; // delayed input    x1
    din_2_            = 0.0; // delayed input    x2
    dout_1_           = 0.0; // delayed output   y1
    dout_2_           = 0.0; // delayed output   y2
    all_output_       = 0.0; // all pass output  y0
    out_bandpass_     = 0.0; // bandpass output
    out_bandreject_   = 0.0; // bandreject output
    sr_               = sample_rate;
    return;
}

void Soap::Process(float in)
{
    // recalculate the coefficients, later move this to a lookup table
    float d = -std::cos(2.0 * PI * (soap_center_freq_ / sr_));

    // tangent bandwidth
    float tf = std::tan(PI * (soap_bandwidth_ / sr_));

    // coefficient
    float c = (tf - 1.0) / (tf + 1.0);

    in_0_ = in;

    all_output_ = -c * in_0_ + (d - d * c) * din_1_ + din_2_
                  - (d - d * c) * dout_1_ + c * dout_2_;

    // move samples in delay for next sample
    din_2_  = din_1_;
    din_1_  = in_0_;
    dout_2_ = dout_1_;
    dout_1_ = all_output_;

    // make factor -1.0 to create a bandpass
    out_bandpass_ = (in_0_ + all_output_ * -1.0) * 0.5;

    // make factor +1.0 to create a bandreject
    out_bandreject_ = (in_0_ + all_output_ * 0.99) * 0.5;

    return;
}

void Soap::SetCenterFreq(float f)
{
    soap_center_freq_ = f;
    return;
}

void Soap::SetFilterBandwidth(float b)
{
    soap_bandwidth_ = b;
    return;
}