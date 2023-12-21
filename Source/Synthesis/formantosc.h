/*
Copyright (c) 2020 Electrosmith, Corp, Emilie Gillet

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/

#pragma once
#ifndef DSY_FORMANTOSCILLATOR_H
#define DSY_FORMANTOSCILLATOR_H

#include <stdint.h>
#ifdef __cplusplus

/** @file formantosc.h */

namespace daisysp
{
/**  
       @brief Formant Oscillator Module. 
       @author Ben Sergentanis
       @date Dec 2020 
       Sinewave with aliasing-free phase reset. \n \n
       Ported from pichenettes/eurorack/plaits/dsp/oscillator/formant_oscillator.h \n
       to an independent module. \n
       Original code written by Emilie Gillet in 2016. \n
*/
class FormantOscillator
{
  public:
    FormantOscillator() {}
    ~FormantOscillator() {}

    /** Initializes the FormantOscillator module.
        \param sample_rate - The sample rate of the audio engine being run. 
    */
    void Init(float sample_rate);

    /** Get the next sample
    */
    float Process();

    /** Set the formant frequency.
        \param freq Frequency in Hz
    */
    void SetFormantFreq(float freq);

    /** Set the carrier frequency. This is the "main" frequency.
        \param freq Frequency in Hz
    */
    void SetCarrierFreq(float freq);

    /** Set the amount of phase shift
        \param ps Typically 0-1. Works with other values though, including negative.
    */
    void SetPhaseShift(float ps);

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
    float ps_inc_;

    float sample_rate_;

    //DISALLOW_COPY_AND_ASSIGN(FormantOscillator);
};
} //namespace daisysp
#endif
#endif