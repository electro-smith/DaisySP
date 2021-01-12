#pragma once
#ifndef DSY_DUST_H
#define DSY_DUST_H

#include <stdint.h>
#include <float.h>
#ifdef __cplusplus

/** @file dust.h */

namespace daisysp
{
/**  
	   @brief Dust Module
	   @author Ported by Ben Sergentanis 
	   @date Jan 2021 
	   Randomly Clocked Samples \n \n
	   Ported from pichenettes/eurorack/plaits/dsp/noise/dust.h \n
	   to an independent module. \n
	   Original code written by Emilie Gillet in 2016. \n

*/
class Dust
{
  public:
    Dust() {}
    ~Dust() {}

    void Init() { SetDensity(.5f); }

    float Process()
    {
        float inv_density = 1.0f / density_;
        float u           = GetFloat();
        if(u < density_)
        {
            return u * inv_density;
        }
        return 0.0f;
    }

    void SetDensity(float density)
    {
        density_ = fclamp(density, 0.f, 1.f);
        density_ = -.5f + density_ * .13f;
    }

  private:
    float density_;
    float frac_ = 1.f / FLT_MAX;

    inline float GetFloat()
    {
        rng_state_ = rng_state_ * 1664525L + 1013904223L;
        return static_cast<float>(rng_state_) / 4294967296.0f;
    }
    int32_t rng_state_ = 0x8D5A6285; //default seed
};
} // namespace daisysp
#endif
#endif