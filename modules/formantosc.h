#pragma once
#ifndef DSY_FORMANTOSCILLATOR_H
#define DSY_FORMANTOSCILLATOR_H

#include <stdint.h>
#ifdef __cplusplus

/** @file formantosc.h */

namespace daisysp
{
/**  
       Formant Oscillator Module \n 
	   Sinewave with aliasing-free phase reset. \n
       Ported by Ben Sergentanis, Dec 2020 
       @author Emilie Gillet
       @date 2016
*/
class FormantOscillator
{
  public:
    FormantOscillator() {}
    ~FormantOscillator() {}

  void Init();
  
  void Process(float carrier_frequency, float formant_frequency, float phase_shift, float* out, size_t size);

 private:
  inline float Sine(float phase);
  inline float ThisBlepSample(float t);
  inline float NextBlepSample(float t);

  // Oscillator state.
  float carrier_phase_;
  float formant_phase_;
  float next_sample_;

  // For interpolation of parameters.
  float carrier_frequency_;
  float formant_frequency_;
  float phase_shift_;
  
  //DISALLOW_COPY_AND_ASSIGN(FormantOscillator);
};
} //namespace daisysp
#endif
#endif