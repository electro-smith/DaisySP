#include "bltriangle.h"
#include <math.h>

using namespace daisysp;

void BlTriangle::Init(float sample_rate)
{
    Sampling_Freq_ = sample_rate; //iconst0
    IOTA_ = 0;

    FourOverSr_  = 4.0 / Sampling_Freq_; //fconst1
    HalfSr_ = 0.5 * Sampling_Freq_; //fconst3
    QuarterSr_ = 0.25 * Sampling_Freq_; //fconst4
    SecPerSample_ = 1.0 / Sampling_Freq_; //fconst5
    
    freq_ = 440;
    amp_ = 0.5;

    for (int i = 0; i < 2; i ++)
    {
        fRec0_[i] = fRec1_[i] = fVec1_[i] = 0.0;
    }

    for (int i = 0; i < 4096; i++)
    {
      fVec2_[i] = 0.0;
    }
    
}


      
float BlTriangle::Process()
{   
    float out;
    float fSlow1 = FourOverSr_ * (amp_ * freq_);
    float fSlow3 = HalfSr_ / freq_;
    int   iSlow4 = (int)fSlow3;
    int   iSlow5 = 1 + iSlow4;
    float fSlow6 = iSlow5 - fSlow3;
    float fSlow7 = QuarterSr_ / freq_;
    float fSlow8 = SecPerSample_ * freq_;
    float fSlow9 = fSlow3 - iSlow4; //decimal portion
      
      
    fRec1_[0]    = fmodf((fSlow8 + fRec1_[1]), 1.0);
    float fTemp0 = 2.0 * fRec1_[0] - 1.0;
    fTemp0      *= fTemp0;       //mult faster than fpow for squaring?
    fVec1_[0]    = fTemp0;
    float fTemp1 = fSlow7 * (fTemp0 - fVec1_[1]);
    fVec2_[IOTA_ & 4095] = fTemp1;
    fRec0_[0]    = 0.0 - ((fSlow6 * fVec2_[(IOTA_ - iSlow4) & 4095] + fSlow9 * fVec2_[(IOTA_ - iSlow5) & 4095]) - (.999 * fRec0_[1] + fTemp1));
    
    out          = (float)(fSlow1 * fRec0_[0]);
    fRec1_[1]    = fRec1_[0];
    fRec0_[1]    = fRec0_[0];
    fVec1_[1]    = fVec1_[0];
    IOTA_++;
			   
    return out;
}
