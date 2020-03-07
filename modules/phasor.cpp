//#include <math.h>
#include "dsp.h"
#include "phasor.h"

using namespace daisysp;

void Phasor::SetFreq(float freq) 
{
	freq_ = freq;
	inc_  = ((float)TWOPI_F * freq_) / sample_rate_;
}

float Phasor::Process() 
{
	float out;
	out = phs_ / ((float)TWOPI_F);
	phs_ += inc_;
	if(phs_ > (float)TWOPI_F)
	{
		phs_ -= (float)TWOPI_F;
	}
	if(phs_ < 0.0f)
	{
		phs_ = 0.0f;
	}
	return out;
}
