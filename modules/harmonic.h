#pragma once
#ifndef DSY_HARMONIC_H
#define DSY_HARMONIC_H

#include <stdint.h>
#ifdef __cplusplus

/** @file harmonic.h */
namespace daisysp {

/**  
       Harmonic Oscillator Module based on Chebyshev polynomials \n 
	   Works well for a small number of harmonics. For the higher order harmonics. \n
       We need to reinitialize the recurrence by computing two high harmonics. \n
       Ported by Ben Sergentanis, Dec 2020 
       @author Emilie Gillet
       @date 2016
*/
class HarmonicOscillator {
 public:
  HarmonicOscillator() {}
  ~HarmonicOscillator() {}

  void Init(int num_harmonics);
  void Process(int first_harmonic_index, float frequency, const float* amplitudes, float* out, size_t size);

 private:
	int num_harmonics_;
  // Oscillator state.
  float phase_;

  // For interpolation of parameters.
  float frequency_;
  float amplitude_[32];
};
}  // namespace daisysp
#endif
#endif