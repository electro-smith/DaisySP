/*
Copyright (c) 2020 Electrosmith, Corp

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/

#pragma once
#ifndef DSY_TREMOLO_H
#define DSY_TREMOLO_H

#include <stdint.h>
#ifdef __cplusplus

#include <math.h>
#include "Synthesis/oscillator.h"

/** @file tremolo.h */

namespace daisysp
{
/**  
    @brief Tremolo effect.
    @author Ben Sergentanis
    @date Jan 2021
    Based on https://christianfloisand.wordpress.com/2012/04/18/coding-some-tremolo/ \n
*/
class Tremolo
{
  public:
    Tremolo() {}
    ~Tremolo() {}

    /** Initializes the module
        \param sample_rate  The sample rate of the audio engine being run.
    */
    void Init(float sample_rate);

    /** 
     \param in Input sample.
     \return Next floating point sample.
    */
    float Process(float in);

    /** Sets the tremolo rate.
       \param freq Tremolo freq in Hz.
    */
    void SetFreq(float freq);

    /** Shape of the modulating lfo 
        \param waveform Oscillator waveform. Use Oscillator::WAVE_SIN for example.
    */
    void SetWaveform(int waveform);

    /** How much to modulate your volume.
        \param depth Works 0-1.
    */
    void SetDepth(float depth);


  private:
    float      sample_rate_, dc_os_;
    Oscillator osc_;
};
} // namespace daisysp
#endif
#endif
