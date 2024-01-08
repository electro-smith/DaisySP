/*
Copyright (c) 2020 Electrosmith, Corp, Emilie Gillet

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/

#pragma once
#ifndef DSY_VARISAWOSCILLATOR_H
#define DSY_VARISAWOSCILLATOR_H

#include <stdint.h>
#ifdef __cplusplus

/** @file variablesawosc.h */
namespace daisysp
{
/**  
       @brief Variable Saw Oscillator. 
       @author Ben Sergentanis
       @date Dec 2020 
       Saw with variable slope or notch. \n \n
       Ported from pichenettes/eurorack/plaits/dsp/oscillator/variable_saw_oscillator.h \n 
       \n to an independent module. \n
       Original code written by Emilie Gillet in 2016. \n
*/
class VariableSawOscillator
{
  public:
    VariableSawOscillator() {}
    ~VariableSawOscillator() {}

    void Init(float sample_rate);

    /** Get the next sample */
    float Process();

    /** Set master freq.
        \param frequency Freq in Hz.
    */
    void SetFreq(float frequency);

    /** Adjust the wave depending on the shape
        \param pw Notch or slope. Works best -1 to 1.
    */
    void SetPW(float pw);

    /** Slope or notch
        \param waveshape 0 = notch, 1 = slope
    */
    void SetWaveshape(float waveshape);


  private:
    float ComputeNaiveSample(float phase,
                             float pw,
                             float slope_up,
                             float slope_down,
                             float triangle_amount,
                             float notch_amount);

    float sample_rate_;

    // Oscillator state.
    float phase_;
    float next_sample_;
    float previous_pw_;
    bool  high_;

    const float kVariableSawNotchDepth = 0.2f;

    // For interpolation of parameters.
    float frequency_;
    float pw_;
    float waveshape_;
};
} // namespace daisysp
#endif
#endif