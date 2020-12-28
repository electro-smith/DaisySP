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

    void Init(float sample_rate, int num_harmonics);
    float
    Process();
	void SetFreq(float freq);
	void SetFirstHarmIdx(int idx);
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