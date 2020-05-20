#include "blsquare.h"
#include <math.h>

using namespace daisysp;

void BlSquare::Init(float sample_rate)
{
    Sampling_Freq_ = sample_rate; //fConst0
    QuarterSr_    = Sampling_Freq_ * 0.25; //fConst1
    SecPerSample_ = 1.0 / Sampling_Freq_;  //fConst2
    
    freq_ = 440;
    amp_  = 0.5;
    pw_   = 0.5;
    IOTA_ = 0;
    
    for (int i = 0; i < 2; i ++)
    {
        fRec0_[i] = fVec1_[i] = fVec2_[i] = 0.0;
    }
} 

      
float BlSquare::Process()
{   
    float out;
    float fSlow2 = Sampling_Freq_ * (pw_ / freq_);
    float fSlow6 = (QuarterSr_ / freq_);
    float fSlow7 = (SecPerSample_ * freq_);
    
    fRec0_[0]    = fmodf(fRec0_[1] + fSlow7, 1.0);
    float fTemp0 = 2.0 * fRec0_[0] - 1.0;
    fTemp0      *= fTemp0;       //mult faster than fpow for squaring?
    fVec1_[0]    = fTemp0;    
    float fTemp1 = (fSlow6 * ((fTemp0 - fVec1_[1])));
    fVec2_[IOTA_ & 4095] = fTemp1;

    out          = (amp_ * (0.0 - (fVec2_[(IOTA_ - (int)fSlow2) & 4095] + - fTemp1)));
    fRec0_[1]    = fRec0_[0];
    fVec1_[1]    = fVec1_[0];
    IOTA_++;
			   
    return out;
}

//slider0    amp
//slider1    pw
//slider2    freq

//fconst0 sr
//fconst1 .25 * sr
//fconst2 1 / sr
