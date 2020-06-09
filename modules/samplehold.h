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

    /** Track and hold circuit. Both circuits are updated in parallel.
    \param gate Tracks new values as long as gate is true.
    \param input Value to track.
    \return Tracked and held value.
    */
    inline float Track(bool gate, float input)
    {
        Update(gate, input);
        return sample_;
    }

    /** Sample and hold. Both track and sample are updated in parallel.
    \param trigger Samples input on rising edge. i.e. Once for each time trigger is true.
    \param input Value to sample and hold.
    \return Sampled and held value.
    */
    inline float Sample(bool trigger, float input)
    {
        Update(trigger, input);
        return track_;
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
