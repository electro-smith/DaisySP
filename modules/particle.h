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
class Particle {
 public:
  Particle() { }
  ~Particle() { }

  inline void Init(float sample_rate);
  
  inline float Process(
      bool sync,
      float density,
      float gain,
      float frequency,
      float spread,
      float q,
      float* out,
      float* aux,
      size_t size);
	  
 private:
	float sample_rate_;

 float pre_gain_;
  Svf filter_;
};
}  // namespace daisysp
#endif
#endif