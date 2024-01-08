/*
Copyright (c) 2020 Electrosmith, Corp

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/

#pragma once
#ifndef DSY_FM2_H
#define DSY_FM2_H

#include <stdint.h>
#include "Synthesis/oscillator.h"
#ifdef __cplusplus

namespace daisysp
{
/** Simple 2 operator FM synth voice.
     
    Date: November, 2020

    Author: Ben Sergentanis
*/

class Fm2
{
  public:
    Fm2() {}
    ~Fm2() {}

    /** Initializes the FM2 module.
        \param samplerate - The sample rate of the audio engine being run. 
    */
    void Init(float samplerate);


    /**  Returns the next sample
    */
    float Process();

    /** Carrier freq. setter
        \param freq Carrier frequency in Hz
    */
    void SetFrequency(float freq);

    /** Set modulator freq. relative to carrier
        \param ratio New modulator freq = carrier freq. * ratio
    */
    void SetRatio(float ratio);

    /** Index setter
      \param FM depth, 5 = 2PI rads
  */
    void SetIndex(float index);

    /** Returns the current FM index. */
    float GetIndex();

    /** Resets both oscillators */
    void Reset();

  private:
    static constexpr float kIdxScalar      = 0.2f;
    static constexpr float kIdxScalarRecip = 1.f / kIdxScalar;

    Oscillator mod_, car_;
    float      idx_;
    float      freq_, lfreq_, ratio_, lratio_;
};
} // namespace daisysp
#endif
#endif
