#pragma once
#ifndef DSY_PARTICLE_H
#define DSY_PARTICLE_H

#include "Svf.h"
#include <stdint.h>
#ifdef __cplusplus

/** @file particle.h */

namespace daisysp
{
/**  
       Random impulse train processed by a resonant filter.
       Ported by Ben Sergentanis, Jan 2021 
       @author Emilie Gillet
       @date 2016
*/
class Particle
{
  public:
    Particle() {}
    ~Particle() {}

    inline void Init(float sample_rate);

    inline float Process(bool   sync,
                         float  density,
                         float  gain,
                         float  frequency,
                         float  spread,
                         float  q);
						 
	float ProcessAux();
						 
  private:
    float sample_rate_;
    float aux_;

    //from stmlib random.h
    inline float GetFloat();
    int32_t      rng_state_ = 0x8D5A61A4; //default seed

    const float ratiofrac_   = 1.f / 12.f;

    float pre_gain_;
    Svf   filter_;
};
} // namespace daisysp
#endif
#endif