#include "blsaw.h"
#include <math.h>

using namespace daisysp;

void BlSaw::Init(float sample_rate)
{
    Sampling_Freq_ = sample_rate; //iconst0, fconst1
    TwoOverSr_ =(float) (2.0 / Sampling_Freq_); // fconst2
    
    freq_ = 440;
    amp_ = 0.5;

    for (int i = 0; i < 2; i ++)
    {
        fRec0_[i] = fVec0_[i] = fVec1_[i] = 0.0;
    }
} 

      
float BlSaw::Process()
{   
    float out;
    float fSlow1  = Sampling_Freq_ * (amp_ / freq_);
    float fSlow2  = (TwoOverSr_ * freq_);
    float fSlow3  = (Sampling_Freq_ / freq_);

    fRec0_[0]    = fmod((1.0 + fRec0_[1]), fSlow3);
    float fTemp0 = fSlow2 * fRec0_[0] - 1.0;
    fTemp0      *= fTemp0;       //mult faster than fpow for squaring?
    fVec0_[0]    = fTemp0;
    fVec1_[0]    = 0.25;
    out          = (float)(fSlow1 * ((fTemp0 - fVec0_[1]) * fVec1_[1]));
    fRec0_[1]    = fRec0_[0];
    fVec0_[1]    = fVec0_[0];
    fVec1_[1]    = fVec1_[0];
    
    return out;
}
