#include <math.h>
#include "metro.h"

using namespace daisysp;

void Metro::Init(float freq, float sample_rate) 
{
	freq_ = freq;
	phs_  = 0.0f;
	sample_rate_ = sample_rate;
	phs_inc_ = (2.0f * M_PI * freq_) / sample_rate_;
}

uint8_t Metro::Process() 
{
	phs_ += phs_inc_;
	if(phs_ >= 2.0f * M_PI) 
	{
		phs_ -= (2.0f * M_PI);
		return 1;
	}
	return 0;
}

void Metro::SetFreq(float freq) 
{
	freq_ = freq;
	phs_inc_ = (2.0f * M_PI * freq_) / sample_rate_;
}
