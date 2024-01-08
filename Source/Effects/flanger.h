/*
Copyright (c) 2020 Electrosmith, Corp

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/

#pragma once
#ifndef DSY_FLANGER_H
#define DSY_FLANGER_H
#ifdef __cplusplus

#include <stdint.h>
#include "Utility/delayline.h"

/** @file flanger.h */

namespace daisysp
{
/** @brief Flanging Audio Effect 
 *
 * Generates a modulating phase shifted copy of a signal, and recombines
 * with the original to create a 'flanging' sound effect.
 */
class Flanger
{
  public:
    /** Initialize the modules
        \param sample_rate Audio engine sample rate.
    */
    void Init(float sample_rate);

    /** Get the next sample
        \param in Sample to process
    */
    float Process(float in);

    /** How much of the signal to feedback into the delay line.
        \param feedback Works 0-1.
    */
    void SetFeedback(float feedback);

    /** How much to modulate the delay by.
        \param depth Works 0-1.
    */
    void SetLfoDepth(float depth);

    /** Set lfo frequency.
        \param freq Frequency in Hz
    */
    void SetLfoFreq(float freq);

    /** Set the internal delay rate. 
        \param delay Tuned for 0-1. Maps to .1 to 7 ms.
    */
    void SetDelay(float delay);

    /** Set the delay time in ms.
        \param ms Delay time in ms, .1 to 7 ms.
    */
    void SetDelayMs(float ms);

  private:
    float                    sample_rate_;
    static constexpr int32_t kDelayLength = 960; // 20 ms at 48kHz = .02 * 48000

    float feedback_;

    //triangle lfos
    float lfo_phase_;
    float lfo_freq_;
    float lfo_amp_;

    float delay_;

    DelayLine<float, kDelayLength> del_;

    float ProcessLfo();
};
} //namespace daisysp
#endif
#endif
