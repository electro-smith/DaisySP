#pragma once
#ifndef DSY_SAMPLEHOLD_H
#define DSY_SAMPLEHOLD_H

#include <stdint.h>
#ifdef __cplusplus

namespace daisysp
{
class SampleHold
{
  public:
    SampleHold() {}
    ~SampleHold() {}

    inline float Process(bool trigger, float input)
    {
        if(trigger)
        {
            value_ = input;
        }

        return value_;
    }

  private:
    float value_ = 0;
};
} // namespace daisysp
#endif
#endif
