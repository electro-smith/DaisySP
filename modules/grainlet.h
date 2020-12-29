#pragma once
#ifndef DSY_GRAINLET_H
#define DSY_GRAINLET_H

#include <stdint.h>
#ifdef __cplusplus

/** @file grainlet.h */
namespace daisysp
{
/**  
       Granular Oscillator Module \n 
	   A phase-distorted single cycle sine * another continuously running sine, \n
	   the whole thing synced to a main oscillator. \n
       Ported by Ben Sergentanis, Dec 2020 
       @author Emilie Gillet
       @date 2016
*/

class GrainletOscillator
{
  public:
    GrainletOscillator() {}
    ~GrainletOscillator() {}

    void Init();

    float Process(float  carrier_frequency,
                  float  formant_frequency,
                  float  carrier_shape,
                  float  carrier_bleed,
                  float* out,
                  size_t size);

  private:
    float Sine(float phase);

    float Carrier(float phase, float shape);

    float Grainlet(float carrier_phase,
                   float formant_phase,
                   float shape,
                   float bleed);

    float ThisBlepSample(float t);
    float NextBlepSample(float t);

    // Oscillator state.
    float carrier_phase_;
    float formant_phase_;
    float next_sample_;

    // For interpolation of parameters.
    float carrier_frequency_;
    float formant_frequency_;
    float carrier_shape_;
    float carrier_bleed_;
};
} // namespace daisysp
#endif
#endif