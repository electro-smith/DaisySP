/*
Copyright (c) 2020 Electrosmith, Corp, Emilie Gillet

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/

#pragma once
#ifndef DSY_VOSIM_H
#define DSY_VOSIM_H

#include <stdint.h>
#ifdef __cplusplus

/** @file vosim.h */

namespace daisysp
{
/**  
         @brief Vosim Oscillator Module \n 
       @author Ben Sergentanis
       @date Dec 2020 
       Two sinewaves multiplied by and sync'ed to a carrier. \n \n
       Ported from pichenettes/eurorack/plaits/dsp/oscillator/vosim_oscillator.h \n 
       \n to an independent module. \n
       Original code written by Emilie Gillet in 2016. \n
*/
class VosimOscillator
{
  public:
    VosimOscillator() {}
    ~VosimOscillator() {}

    /** Initializes the FormantOscillator module.
        \param sample_rate - The sample rate of the audio engine being run. 
    */
    void Init(float sample_rate);

    /** Get the next sample
    */
    float Process();

    /** Set carrier frequency.
        \param freq Frequency in Hz.
    */
    void SetFreq(float freq);

    /** Set formant 1 frequency.
        \param freq Frequency in Hz.
    */
    void SetForm1Freq(float freq);

    /** Set formant 2 frequency.
        \param freq Frequency in Hz.
    */
    void SetForm2Freq(float freq);

    /** Waveshaping
        \param shape Shape to set. Works -1 to 1
    */
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