#pragma once
#ifndef DSY_SMOOTHRANDOM_H
#define DSY_SMOOTHRANDOM_H

#include "dsp.h"
#include <stdint.h>
#ifdef __cplusplus

/** @file smooth_random.h */

namespace daisysp
{
/**  
		Smooth random generator for the internal modulations \n
       Ported by Ben Sergentanis, Jan 2021 
       @author Emilie Gillet
       @date 2016
*/
class SmoothRandomGenerator {
 public:
  SmoothRandomGenerator() { }
  ~SmoothRandomGenerator() { }
  
 void Init(float sample_rate) {
	sample_rate_ = sample_rate;
	Seed(0x8D5A61A4);
	
	SetFreq(1.f);
    phase_ = 0.0f;
    from_ = 0.0f;
    interval_ = 0.0f;
  }
  
  float Process() {
    phase_ += frequency_;
    if (phase_ >= 1.0f) {
      phase_ -= 1.0f;
      from_ += interval_;
      interval_ = GetFloat() * 2.0f - 1.0f - from_;
    }
    float t = phase_ * phase_ * (3.0f - 2.0f * phase_);
    return from_ + interval_ * t;
  }

	void SetFreq(float freq){
		freq = freq / sample_rate_;
		frequency_ = fclamp(freq, 0.f, 1.f);
	}

	void Seed(uint32_t seed){
		rng_state_ = seed;
	}
  
 private:
  float frequency_;
  float phase_;
  float from_;
  float interval_;
  
  float sample_rate_;
  
  //from stmlib random.h
inline float GetFloat()
{
    rng_state_ = rng_state_ * 1664525L + 1013904223L;
    return static_cast<float>(rng_state_) / 4294967296.0f;
}
  int32_t      rng_state_;
};

}  // namespace daisysp
#endif
#endif