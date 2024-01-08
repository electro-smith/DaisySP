/*
Copyright (c) 2020 Electrosmith, Corp

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/

#pragma once
#ifndef DSY_METRO_H
#define DSY_METRO_H
#include <stdint.h>
#ifdef __cplusplus

namespace daisysp
{
/** Creates a clock signal at a specific frequency.
*/
class Metro
{
  public:
    Metro() {}
    ~Metro() {}
    /** Initializes Metro module.
        Arguments:
        - freq: frequency at which new clock signals will be generated
            Input Range: 
        - sample_rate: sample rate of audio engine
            Input range: 
    */
    void Init(float freq, float sample_rate);

    /** checks current state of Metro object and updates state if necesary.
    */
    uint8_t Process();

    /** resets phase to 0
    */
    inline void Reset() { phs_ = 0.0f; }
    /** Sets frequency at which Metro module will run at.
    */
    void SetFreq(float freq);

    /** Returns current value for frequency.
    */
    inline float GetFreq() { return freq_; }

  private:
    float freq_;
    float phs_, sample_rate_, phs_inc_;
};
} // namespace daisysp
#endif
#endif
