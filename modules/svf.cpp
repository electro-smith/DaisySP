#include <math.h>
#include "svf.h"

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

using namespace daisysp;

void Svf::Init(float sample_rate)
{
    sr_        = sample_rate;
    fc_        = 200.0f;
    res_       = 0.5f;
    drive_     = 0.5f;
    freq_      = 0.25f;
    damp_      = 0.0f;
    notch_     = 0.0f;
    low_       = 0.0f;
    high_      = 0.0f;
    band_      = 0.0f;
    peak_      = 0.0f;
    input_     = 0.0f;
    out_notch_ = 0.0f;
    out_low_   = 0.0f;
    out_high_  = 0.0f;
    out_peak_  = 0.0f;
    out_band_  = 0.0f;
}

void Svf::Process(float in)
{
    input_ = in;
    // first pass
    notch_     = input_ - damp_ * band_;
    low_       = low_ + freq_ * band_;
    high_      = notch_ - low_;
    band_      = freq_ * high_ + band_ - drive_ * band_ * band_ * band_;
    out_low_   = 0.5f * low_;
    out_high_  = 0.5f * high_;
    out_band_  = 0.5f * band_;
    out_peak_  = 0.5f * (low_ - high_);
    out_notch_ = 0.5f * notch_;
    // second pass
    notch_ = input_ - damp_ * band_;
    low_   = low_ + freq_ * band_;
    high_  = notch_ - low_;
    band_  = freq_ * high_ + band_ - drive_ * band_ * band_ * band_;
    out_low_ += 0.5f * low_;
    out_high_ += 0.5f * high_;
    out_band_ += 0.5f * band_;
    out_peak_ += 0.5f * (low_ - high_);
    out_notch_ += 0.5f * notch_;
}

void Svf::SetFreq(float f)
{
    if(f < 0.000001f)
    {
        fc_ = 0.000001f;
    }
    else if(f > sr_ / 2.0f)
    {
        fc_ = (sr_ / 2.0f) - 1.0f;
    }
    else
    {
        fc_ = f;
    }
    // Set Internal Frequency for fc_
    freq_ = 2.0f
            * sinf((float)M_PI
                   * MIN(0.25f,
                         fc_ / (sr_ * 2.0f))); // fs*2 because double sampled
    // recalculate damp
    //damp = (MIN(2.0f * powf(res_, 0.25f), MIN(2.0f, 2.0f / freq - freq * 0.5f)));
    damp_ = MIN(2.0f * (1.0f - powf(res_, 0.25f)),
                MIN(2.0f, 2.0f / freq_ - freq_ * 0.5f));
}

void Svf::SetRes(float r)
{
    if(r < 0.0f)
    {
        r = 0.0f;
    }
    else if(r > 1.0f)
    {
        r = 1.0f;
    }
    res_ = r;
    // recalculate damp
    //damp = (MIN(2.0f * powf(res_, 0.25f), MIN(2.0f, 2.0f / freq - freq * 0.5f)));
    damp_ = MIN(2.0f * (1.0f - powf(res_, 0.25f)),
                MIN(2.0f, 2.0f / freq_ - freq_ * 0.5f));
}
