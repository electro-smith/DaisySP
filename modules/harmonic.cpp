#include "dsp.h"
#include "harmonic.h"

using namespace daisysp;

void HarmonicOscillator::Init(float sample_rate, int num_harmonics)
{
	sample_rate_ = sample_rate;
    phase_         = 0.0f;
    frequency_     = 0.0f;
    num_harmonics_ = num_harmonics > MAX_HARMS ? MAX_HARMS : num_harmonics;
    for(int i = 0; i < num_harmonics_; ++i)
    {
        amplitude_[i] = 0.0f;
		newamplitude_[i] = 0.f;
    }
	recalc_ = false;
}

float HarmonicOscillator::Process()
{
	if (recalc_){
		recalc_ = false;
		for(int i = 0; i < num_harmonics_; ++i)
		{
			float f = frequency_ * static_cast<float>(first_harmonic_index_ + i);
			if(f >= 0.5f)
			{
				f = 0.5f;
			}
			amplitude_[i] = newamplitude_[i] * (1.0f - f * 2.0f);
		}
	}
	
    phase_ += frequency_;
    if(phase_ >= 1.0f)
    {
        phase_ -= 1.0f;
    }
    const float two_x = 2.0f * sinf(phase_ * TWOPI_F);
    float       previous, current;
    if(first_harmonic_index_ == 1)
    {
        previous = 1.0f;
        current  = two_x * 0.5f;
    }
    else
    {
        const float k = first_harmonic_index_;
        previous      = sinf((phase_ * (k - 1.0f) + 0.25f) * TWOPI_F);
        current       = sinf((phase_ * k) * TWOPI_F);
    }

    float sum = 0.0f;
    for(int i = 0; i < num_harmonics_; ++i)
    {
        sum += amplitude_[i] * current;
        float temp = current;
        current    = two_x * current - previous;
        previous   = temp;
    }

    return sum;
}

void HarmonicOscillator::SetFreq(float freq){
	recalc_ = true;
    //convert from Hz to phase_inc / sample
    frequency_ = freq / sample_rate_;
    frequency_ = frequency_ >= .5f ? .5f : frequency_;
    frequency_ = frequency_ <= -.5f ? -.5f :frequency_;
}

void HarmonicOscillator::SetFirstHarmIdx(int idx){
	recalc_ = true;
	first_harmonic_index_ = idx;
}

void HarmonicOscillator::SetAmplitudes(const float* amplitudes){
	recalc_ = true;
	for(int i = 0; i < num_harmonics_; i++){
		newamplitude_[i] = amplitudes[i];
	}
}
