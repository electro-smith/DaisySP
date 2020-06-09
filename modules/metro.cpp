#include <math.h>
#include "metro.h"

#ifndef MAX_32
#define MAX_32 0xFFFFFFFFU
#endif

using namespace daisysp;

void Metro::Init(float freq, float sample_rate)
{
    freq_        = freq;
    phs_         = 0.0f;
    sample_rate_ = sample_rate;
    phs_inc_     = (2.0f * M_PI * freq_) / sample_rate_;

    max_tap_     = MAX_32; //about 25 hours at 48kHz
}

//Increments tap counter, forces tap if counter maxes out
void Metro::TapTimer()
{
    if (tap_active_)
    {
        tap_count_++;
	if (tap_count_ == max_tap_)
	{
	    Tap();
	}
    }
}

uint8_t Metro::Process()
{
    TapTimer();
  
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
    freq_    = freq;
    phs_inc_ = (2.0f * M_PI * freq_) / sample_rate_;
}

void Metro::Tap()
{
    tap_active_ = ! tap_active_;
    if (!tap_active_)
    {
        SetFreq(sample_rate_ / tap_count_);
        tap_count_ = 0;
    }
}
