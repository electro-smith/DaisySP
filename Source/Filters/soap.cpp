#include "soap.h"
#include <math.h>

#define PI 3.141592653589793

void Soap::Init(float sample_rate)
{
    soap_center_freq_ =        400.0;
    soap_bandwidth_ =          50.0;
    in_0_ =                    0.0;          // input            x0
    din_1_ =                   0.0;          // delayed input    x1
    din_2_ =                   0.0;          // delayed input    x2
    dout_1_ =                  0.0;          // delayed output   y1
    dout_2_ =                  0.0;          // delayed output   y2
    all_output_ =              0.0;          // all pass output  y0
    sr_ = sample_rate;
    return;
}

float Soap::Process(float in)
{
    // recalculate the coefficients, later move this to a lookup table
    double d = -cos(2.0 * PI * (soap_center_freq_/sr_));
    
    // tangent bandwidth
    double tf = tan(PI * (soap_bandwidth_/sr_));

    // coefficient     
    double c = (tf - 1.0)/(tf + 1.0);                              

    in_0_ = in;   
    
    all_output_ = -c*in_0_ + (d - d*c)*din_1_ + din_2_ - (d - d*c)*dout_1_ + c*dout_2_;

    // move samples in delay for next sample
    din_2_ = din_1_;
    din_1_ = in_0_;
    dout_2_ = dout_1_;
    dout_1_ = all_output_;

    // make factor -1.0 to create a bandpass
    return (in_0_ + all_output_ * -1.0) * 0.5;            
}

void Soap::SetCenterFreq(float f)
{
    soap_center_freq_ = f;
    return;
}

void Soap::SetFilterBandwidth(float b) {
    soap_bandwidth_ = b;
    return;
}