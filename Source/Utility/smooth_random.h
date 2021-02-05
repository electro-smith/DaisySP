#pragma once
#ifndef DSY_SMOOTHRANDOM_H
#define DSY_SMOOTHRANDOM_H

#include "dsp.h"
#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus

/** @file smooth_random.h */

namespace daisysp
{
/**  
       @brief Smooth random generator for internal modulation. \n
       @author Ported by Ben Sergentanis 
       @date Jan 2021 
       Ported from pichenettes/eurorack/plaits/dsp/noise/smooth_random_generator.h \n
       to an independent module. \n
       Original code written by Emilie Gillet in 2016. \n
*/
class SmoothRandomGenerator
{
  public:
    SmoothRandomGenerator() {}
    ~SmoothRandomGenerator() {}

    /** Initialize the module
        \param sample_rate Audio engine sample rate.
    */
    void Init(float sample_rate)
    {
        sample_rate_ = sample_rate;

        SetFreq(1.f);
        phase_    = 0.0f;
        from_     = 0.0f;
        interval_ = 0.0f;
    }

    /** Get the next float. Ranges from -1 to 1. */
    float Process()
    {
        phase_ += frequency_;
        if(phase_ >= 1.0f)
        {
            phase_ -= 1.0f;
            from_ += interval_;
            interval_ = rand() * kRandFrac * 2.0f - 1.0f - from_;
        }
        float t = phase_ * phase_ * (3.0f - 2.0f * phase_);
        return from_ + interval_ * t;
    }

    /** How often to slew to a new random value
        \param freq Rate in Hz
    */
    void SetFreq(float freq)
    {
        freq       = freq / sample_rate_;
        frequency_ = fclamp(freq, 0.f, 1.f);
    }

  private:
    float frequency_;
    float phase_;
    float from_;
    float interval_;

    float sample_rate_;

    static constexpr float kRandFrac = 1.f / (float)RAND_MAX;
};

} // namespace daisysp
#endif
#endif