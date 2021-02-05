#pragma once
#ifndef DSY_HARMONIC_H
#define DSY_HARMONIC_H

#include <stdint.h>
#include "Utility/dsp.h"
#ifdef __cplusplus


/** @file harmonic_osc.h */
namespace daisysp
{
/**  
       @brief Harmonic Oscillator Module based on Chebyshev polynomials. 
       @author Ben Sergentanis
       @date Dec 2020 
       Harmonic Oscillator Module based on Chebyshev polynomials \n 
       Works well for a small number of harmonics. For the higher order harmonics. \n
       We need to reinitialize the recurrence by computing two high harmonics. \n \n
       Ported from pichenettes/eurorack/plaits/dsp/oscillator/harmonic_oscillator.h \n
       to an independent module. \n
       Original code written by Emilie Gillet in 2016. \n
*/
template <int num_harmonics = 16>
class HarmonicOscillator
{
  public:
    HarmonicOscillator() {}
    ~HarmonicOscillator() {}

    /** Initialize harmonic oscillator
        \param sample_rate Audio engine samplerate
    */
    void Init(float sample_rate)
    {
        sample_rate_ = sample_rate;
        phase_       = 0.0f;

        for(int i = 0; i < num_harmonics; ++i)
        {
            amplitude_[i]    = 0.0f;
            newamplitude_[i] = 0.f;
        }
        amplitude_[0]    = 1.f;
        newamplitude_[0] = 1.f;

        SetFirstHarmIdx(1);
        SetFreq(440.f);

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
        //convert from Hz to phase_inc / sample
        freq       = freq / sample_rate_;
        freq       = freq >= .5f ? .5f : freq;
        freq       = freq <= -.5f ? -.5f : freq;
        recalc_    = cmp(freq, frequency_) || recalc_;
        frequency_ = freq;
    }

    /** Offset the set of harmonics. Passing in 3 means "harmonic 0" is the 3rd harm., 1 is the 4th, etc.
        \param idx Default behavior is 1. Values < 0 default to 1.
    */
    void SetFirstHarmIdx(int idx)
    {
        idx                   = idx < 1 ? 1 : idx;
        recalc_               = cmp(idx, first_harmonic_index_) || recalc_;
        first_harmonic_index_ = idx;
    }

    /** Set the amplitudes of each harmonic of the root. 
        \param amplitudes Amplitudes to set. Sum of all amplitudes must be < 1. The array referenced must be at least as large as num_harmonics. 
    */
    void SetAmplitudes(const float* amplitudes)
    {
        for(int i = 0; i < num_harmonics; i++)
        {
            recalc_          = cmp(newamplitude_[i], amplitudes[i]) || recalc_;
            newamplitude_[i] = amplitudes[i];
        }
    }

    /** Sets one amplitude. Does nothing if idx out of range.
        \param amp Amplitude to set
        \param idx Which harmonic to set.
    */
    void SetSingleAmp(const float amp, int idx)
    {
        if(idx < 0 || idx >= num_harmonics)
        {
            return;
        }
        recalc_            = cmp(amplitude_[idx], amp) || recalc_;
        newamplitude_[idx] = amp;
    }


  private:
    bool cmp(float a, float b) { return fabsf(a - b) > .000001f; }

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