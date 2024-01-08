/*
Copyright (c) 2020 Electrosmith, Corp, Emilie Gillet

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/

#pragma once
#ifndef DSY_SR_REDUCER_H
#define DSY_SR_REDUCER_H

#include <stdint.h>
#ifdef __cplusplus

/** @file sampleratereducer.h */

namespace daisysp
{
/** 
       @brief Sample rate reducer.
       @author Ben Sergentanis
       @date Jan 2021
       Ported from pichenettes/eurorack/plaits/dsp/fx/sample_rate_reducer.h \n
       to an independent module. \n
       Original code written by Emilie Gillet in 2014. \n
*/
class SampleRateReducer
{
  public:
    SampleRateReducer() {}
    ~SampleRateReducer() {}

    /** Initialize the module */
    void Init();

    /** Get the next floating point sample 
        \param in Sample to be processed.
    */
    float Process(float in);

    /** Set the new sample rate.
        \param frequency over 0-1. 1 is full quality, .5 is half sample rate, etc.
    */
    void SetFreq(float frequency);

  private:
    float frequency_;
    float phase_;
    float sample_;
    float previous_sample_;
    float next_sample_;
};
} // namespace daisysp
#endif
#endif
