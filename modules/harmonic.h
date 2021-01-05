#pragma once
#ifndef DSY_HARMONIC_H
#define DSY_HARMONIC_H

#include <stdint.h>
#include "dsp.h"
#ifdef __cplusplus


/** @file harmonic.h */
namespace daisysp
{
/**  
       Harmonic Oscillator Module based on Chebyshev polynomials \n 
	   Works well for a small number of harmonics. For the higher order harmonics. \n
       We need to reinitialize the recurrence by computing two high harmonics. \n
       Ported by Ben Sergentanis, Dec 2020 
       @author Emilie Gillet
       @date 2016
*/
template <int num_harmonics>
class HarmonicOscillator
{
  public:
    HarmonicOscillator() {}
    ~HarmonicOscillator() {}

    /** Initialize harmonic oscillator
		\param sample_rate Audio engine samplerate
		\param num_harmonics How many harmonics we'll use. Max of 32.
	*/
    void Init(float sample_rate)
    {
        sample_rate_ = sample_rate;
        phase_       = 0.0f;
        frequency_   = 0.0f;
        for(int i = 0; i < num_harmonics; ++i)
        {
            amplitude_[i]    = 0.0f;
            newamplitude_[i] = 0.f;
        }
        recalc_ = false;
    }

    /** Get the next floating point sample */
    float Process()
    {
        if(recalc_)
        {
            recalc_ = false;
            for(int i = 0; i < num_harmonics; ++i)
            {
                float f = frequency_
                          * static_cast<float>(first_harmonic_index_ + i);
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
        for(int i = 0; i < num_harmonics; ++i)
        {
            sum += amplitude_[i] * current;
            float temp = current;
            current    = two_x * current - previous;
            previous   = temp;
        }

        return sum;
    }

    /** Set the main frequency 
		\param freq Freq to be set in Hz.
	*/
    void SetFreq(float freq)
    {
        recalc_ = true;
        //convert from Hz to phase_inc / sample
        frequency_ = freq / sample_rate_;
        frequency_ = frequency_ >= .5f ? .5f : frequency_;
        frequency_ = frequency_ <= -.5f ? -.5f : frequency_;
    }
    /** Offset the set of harmonics. For example, passing in 3 would mean "harmonic 0" is actually the 3rd, 1 is the 4th, etc.
		\param idx Which harmonic of the root will be the "first" harmonic.
	*/
    void SetFirstHarmIdx(int idx)
    {
        recalc_               = true;
        first_harmonic_index_ = idx;
    }
    /** Set the amplitudes of each harmonic of the root. 
		\param amplitudes Amplitudes to set. Sum of all amplitudes must be < 1. The array referenced must be at least as large as num_harmonics. 
	*/
    void SetAmplitudes(const float* amplitudes)
    {
        recalc_ = true;
        for(int i = 0; i < num_harmonics; i++)
        {
            newamplitude_[i] = amplitudes[i];
        }
    }

  private:
    float sample_rate_;
    float phase_;
    float frequency_;
    float amplitude_[num_harmonics];
    float newamplitude_[num_harmonics];
    bool  recalc_;

    int first_harmonic_index_;
};
} // namespace daisysp
#endif
#endif