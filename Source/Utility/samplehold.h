/*
Copyright (c) 2020 Electrosmith, Corp, Paul Batchelor

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/

#pragma once
#ifndef DSY_SAMPLEHOLD_H
#define DSY_SAMPLEHOLD_H

#include <stdint.h>
#ifdef __cplusplus

namespace daisysp
{
/** Dual track and hold / Sample and hold module. \n 
    Ported from soundpipe by Ben Sergentanis, June 2020.
    @author Paul Batchelor
    @date   2015
*/
class SampleHold
{
  public:
    SampleHold() {}
    ~SampleHold() {}

    enum Mode
    {
        MODE_SAMPLE_HOLD,
        MODE_TRACK_HOLD,
        MODE_LAST,
    };

    /** Process the next sample. Both sample and track and hold are run in parallel
    \param trigger Trigger the sample/track and hold
    \param input   Signal to be sampled/tracked and held
    \param mode    Whether to output the tracked or sampled values.
    */
    inline float
    Process(bool trigger, float input, Mode mode = MODE_SAMPLE_HOLD)
    {
        Update(trigger, input);
        return mode == MODE_SAMPLE_HOLD ? sample_ : track_;
    }

  private:
    float track_    = 0;
    float sample_   = 0;
    bool  previous_ = false;


    inline void Update(bool trigger, float input)
    {
        if(trigger)
        {
            if(!previous_)
            {
                sample_ = input;
            }
            track_ = input;
        }
        previous_ = trigger;
    }
};
} // namespace daisysp
#endif
#endif
