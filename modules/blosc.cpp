#include "blosc.h"
#include <math.h>

using namespace daisysp;

void BlOsc::Init(float sample_rate)
{
    Sampling_Freq_ = sample_rate; 
    HalfSr_ = 0.5 * Sampling_Freq_; 
    QuarterSr_    = Sampling_Freq_ * 0.25;
    SecPerSample_ = 1.0 / Sampling_Freq_;  
    TwoOverSr_ =(float) (2.0 / Sampling_Freq_);
    FourOverSr_  = 4.0 / Sampling_Freq_;
    
    freq_ = 440;
    amp_  = 0.5;
    pw_   = 0.5;
    IOTA_ = 0;
    mode_ = BL_WAVEFORM_TRIANGLE;
    
    for (int i = 0; i < 2; i ++)
    {
        fRec0_[i] = fRec1_[i] = fVec0_[i] = fVec1_[i] = 0.0;
    }

    for (int i = 0; i < 4096; i++)
    {
        fVec2_[i] = 0.0;
    }
} 

float BlOsc::ProcessSquare()
{   
    float out;
    float fSlow2 = fmin(2047.0, Sampling_Freq_ * (pw_ / freq_));
    float fSlow5 = (float)((int)fSlow2 + 1) - fSlow2;
    float fSlow6 = (QuarterSr_ / freq_);
    float fSlow7 = (SecPerSample_ * freq_);
    float fSlow8 = fSlow2 - (int)fSlow2;
    
    fRec0_[0]    = fmodf(fRec0_[1] + fSlow7, 1.0);
    float fTemp0 = 2.0 * fRec0_[0] - 1.0;
    fTemp0      *= fTemp0;       //mult faster than fpow for squaring?
    fVec1_[0]    = fTemp0;    
    float fTemp1 = (fSlow6 * ((fTemp0 - fVec1_[1])));
    fVec2_[IOTA_ & 4095] = fTemp1;

    out          = amp_ * (0.0 - ((fSlow5 * fVec2_[(IOTA_ - (int)fSlow2) & 4095] + fSlow8 * fVec2_[(IOTA_ - ((int)fSlow2 + 1)) & 4095]) - fTemp1));
    fRec0_[1]    = fRec0_[0];
    fVec1_[1]    = fVec1_[0];
    IOTA_++;
			   
    return out;
}

float BlOsc::ProcessTriangle()
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

float BlOsc::ProcessSaw()
{   
    float out;
    //fSlow0 = Slider1 = freq
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

void BlOsc::Reset()
{
    IOTA_ = 0;
  
    for (int i = 0; i < 2; i ++)
    {
        fRec0_[i] = fRec1_[i] = fVec0_[i] = fVec1_[i] = 0.0;
    }

    for (int i = 0; i < 4096; i++)
    {
        fVec2_[i] = 0.0;
    }  
}

float BlOsc::Process()
{  
    switch(mode_)
    {
        case BL_WAVEFORM_TRIANGLE:
	    return ProcessTriangle();
	case BL_WAVEFORM_SAW:
            return ProcessSaw();
        case BL_WAVEFORM_SQUARE:
	    return ProcessSquare();
	default:
   	    break;
    }
    
    return 0.0;
}
