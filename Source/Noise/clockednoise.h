/*
Copyright (c) 2020 Electrosmith, Corp, Emilie Gillet

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/

#pragma once
#ifndef DSY_CLOCKEDNOISE_H
#define DSY_CLOCKEDNOISE_H

#include <stdint.h>
#ifdef __cplusplus

/** @file clockednoise.h */

namespace daisysp
{
/**  
           @brief Clocked Noise Module
       @author Ported by Ben Sergentanis 
       @date Jan 2021 
       Noise processed by a sample and hold running at a target frequency. \n \n
       Ported from pichenettes/eurorack/plaits/dsp/noise/clocked_noise.h \n
       to an independent module. \n
       Original code written by Emilie Gillet in 2016. \n
*/
class ClockedNoise
{
  public:
    ClockedNoise() {}
    ~ClockedNoise() {}

    /** Initialize module    
        \param sample_rate Audio engine sample rate
    */
    void Init(float sample_rate);

    /** Get the next floating point sample */
    float Process();

    /** Set the frequency at which the next sample is generated.
        \param freq Frequency in Hz
    */
    void SetFreq(float freq);

    /** Calling this forces another random float to be generated */
    void Sync();

  private:
    // Oscillator state.
    float phase_;
    float sample_;
    float next_sample_;

    // For interpolation of parameters.
    float frequency_;

    float sample_rate_;

    static constexpr float kRandFrac = 1.f / (float)RAND_MAX;
};
} // namespace daisysp
#endif
#endif
