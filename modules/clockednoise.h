#pragma once
#ifndef DSY_CLOCKEDNOISE_H
#define DSY_CLOCKEDNOISE_H

#include <stdint.h>
#ifdef __cplusplus

/** @file clockednoise.h */

namespace daisysp
{
/**  
	   Noise processed by a sample and hold running at a target frequency. \n
       Ported by Ben Sergentanis, Jan 2021 
       @author Emilie Gillet
       @date 2016
*/
class ClockedNoise
{
  public:
    ClockedNoise() {}
    ~ClockedNoise() {}

    void Init();

    float Process(bool sync, float frequency);

  private:
    // Oscillator state.
    float phase_;
    float sample_;
    float next_sample_;

    // For interpolation of parameters.
    float frequency_;


    //from stmlib random.h
    inline float GetFloat();
    int32_t      rng_state_ = 0x8D5A61A4; //default seed
};
} // namespace daisysp
#endif
#endif