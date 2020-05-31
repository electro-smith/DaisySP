#include "blosc.h"
#include <math.h>

using namespace daisysp;

void BlOsc::Init(float sample_rate)
{
    sampling_freq_  = sample_rate;
    half_sr_        = 0.5 * sampling_freq_;
    quarter_sr_     = sampling_freq_ * 0.25;
    sec_per_sample_ = 1.0 / sampling_freq_;
    two_over_sr_    = (float)(2.0 / sampling_freq_);
    four_over_sr_   = 4.0 / sampling_freq_;

    freq_ = 440;
    amp_  = 0.5;
    pw_   = 0.5;
    iota_ = 0;
    mode_ = WAVE_TRIANGLE;

    for(int i = 0; i < 2; i++)
    {
        rec0_[i] = rec1_[i] = vec0_[i] = vec1_[i] = 0.0;
    }

    for(int i = 0; i < 4096; i++)
    {
        vec2_[i] = 0.0;
    }
}

float BlOsc::ProcessSquare()
{
    float out;
    float fSlow2 = fmin(2047.0, sampling_freq_ * (pw_ / freq_));
    float fSlow5 = (float)((int)fSlow2 + 1) - fSlow2;
    float fSlow6 = (quarter_sr_ / freq_);
    float fSlow7 = (sec_per_sample_ * freq_);
    float fSlow8 = fSlow2 - (int)fSlow2;

    rec0_[0]     = fmodf(rec0_[1] + fSlow7, 1.0);
    float fTemp0 = 2.0 * rec0_[0] - 1.0;
    fTemp0 *= fTemp0; //mult faster than fpow for squaring?
    vec1_[0]            = fTemp0;
    float fTemp1        = (fSlow6 * ((fTemp0 - vec1_[1])));
    vec2_[iota_ & 4095] = fTemp1;

    out = amp_
          * (0.0
             - ((fSlow5 * vec2_[(iota_ - (int)fSlow2) & 4095]
                 + fSlow8 * vec2_[(iota_ - ((int)fSlow2 + 1)) & 4095])
                - fTemp1));
    rec0_[1] = rec0_[0];
    vec1_[1] = vec1_[0];
    iota_++;

    return out;
}

float BlOsc::ProcessTriangle()
{
    float out;
    float fSlow1 = four_over_sr_ * (amp_ * freq_);
    float fSlow3 = half_sr_ / freq_;
    int   iSlow4 = (int)fSlow3;
    int   iSlow5 = 1 + iSlow4;
    float fSlow6 = iSlow5 - fSlow3;
    float fSlow7 = quarter_sr_ / freq_;
    float fSlow8 = sec_per_sample_ * freq_;
    float fSlow9 = fSlow3 - iSlow4; //decimal portion


    rec1_[0]     = fmodf((fSlow8 + rec1_[1]), 1.0);
    float fTemp0 = 2.0 * rec1_[0] - 1.0;
    fTemp0 *= fTemp0; //mult faster than fpow for squaring?
    vec1_[0]            = fTemp0;
    float fTemp1        = fSlow7 * (fTemp0 - vec1_[1]);
    vec2_[iota_ & 4095] = fTemp1;
    rec0_[0]            = 0.0
               - ((fSlow6 * vec2_[(iota_ - iSlow4) & 4095]
                   + fSlow9 * vec2_[(iota_ - iSlow5) & 4095])
                  - (.999 * rec0_[1] + fTemp1));

    out      = (float)(fSlow1 * rec0_[0]);
    rec1_[1] = rec1_[0];
    rec0_[1] = rec0_[0];
    vec1_[1] = vec1_[0];
    iota_++;

    return out;
}

float BlOsc::ProcessSaw()
{
    float out;
    //fSlow0 = Slider1 = freq
    float fSlow1 = sampling_freq_ * (amp_ / freq_);
    float fSlow2 = (two_over_sr_ * freq_);
    float fSlow3 = (sampling_freq_ / freq_);

    rec0_[0]     = fmod((1.0 + rec0_[1]), fSlow3);
    float fTemp0 = fSlow2 * rec0_[0] - 1.0;
    fTemp0 *= fTemp0; //mult faster than fpow for squaring?
    vec0_[0] = fTemp0;
    vec1_[0] = 0.25;
    out      = (float)(fSlow1 * ((fTemp0 - vec0_[1]) * vec1_[1]));
    rec0_[1] = rec0_[0];
    vec0_[1] = vec0_[0];
    vec1_[1] = vec1_[0];

    return out;
}

void BlOsc::Reset()
{
    iota_ = 0;

    for(int i = 0; i < 2; i++)
    {
        rec0_[i] = rec1_[i] = vec0_[i] = vec1_[i] = 0.0;
    }

    for(int i = 0; i < 4096; i++)
    {
        vec2_[i] = 0.0;
    }
}

float BlOsc::Process()
{
    switch(mode_)
    {
        case WAVE_TRIANGLE: return ProcessTriangle();
        case WAVE_SAW: return ProcessSaw();
        case WAVE_SQUARE: return ProcessSquare();
        default: break;
    }

    return 0.0;
}
