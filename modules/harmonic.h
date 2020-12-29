#pragma once
#ifndef DSY_HARMONIC_H
#define DSY_HARMONIC_H

#include <stdint.h>
#ifdef __cplusplus

#define MAX_HARMS 32

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
class HarmonicOscillator
{
  public:
    HarmonicOscillator() {}
    ~HarmonicOscillator() {}

    /** Initialize harmonic oscillator
		\param sample_rate Audio engine samplerate
		\param num_harmonics How many harmonics we'll use. Max of 32.
	*/
    void Init(float sample_rate, int num_harmonics);

    /** Get the next floating point sample */
    float Process();

    /** Set the main frequency 
		\param freq Freq to be set in Hz.
	*/
    void SetFreq(float freq);

    /** Offset the set of harmonics. For example, passing in 3 would mean "harmonic 0" is actually the 3rd, 1 is the 4th, etc.
		\param idx Which harmonic of the root will be the "first" harmonic.
	*/
    void SetFirstHarmIdx(int idx);

    /** Set the amplitudes of each harmonic of the root. 
		\param amplitudes Amplitudes to set. Sum of all amplitudes must be < 1. The array referenced must be at least as large as num_harmonics. 
	*/
    void SetAmplitudes(const float* amplitudes);

  private:
    float sample_rate_;
    float phase_;
    float frequency_;
    float amplitude_[MAX_HARMS];
    float newamplitude_[MAX_HARMS];
    bool  recalc_;

    int num_harmonics_;
    int first_harmonic_index_;
};
} // namespace daisysp
#endif
#endif