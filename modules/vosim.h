#pragma once
#ifndef DSY_VOSIM_H
#define DSY_VOSIM_H

#include <stdint.h>
#ifdef __cplusplus

/** @file vosim.h */

namespace daisysp
{
/**  
       Vosim Oscillator Module \n 
	   Two sinewaves multiplied by and sync'ed to a carrier. \n
       Ported by Ben Sergentanis, Dec 2020 
       @author Emilie Gillet
       @date 2016
*/
class VOSIMOscillator
{
  public:
    VOSIMOscillator() {}
    ~VOSIMOscillator() {}

    void Init(float sample_rate);

    float Process();

    void SetCarrierFreq(float freq);

    void SetForm1Freq(float freq);

    void SetForm2Freq(float freq);

    void SetShape(float shape);

  private:
    float Sine(float phase);

    float sample_rate_;

    // Oscillator state.
    float carrier_phase_;
    float formant_1_phase_;
    float formant_2_phase_;

    // For interpolation of parameters.
    float carrier_frequency_;
    float formant_1_frequency_;
    float formant_2_frequency_;
    float carrier_shape_;
};
} // namespace daisysp
#endif
#endif