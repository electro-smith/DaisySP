#pragma once
#ifndef DSY_RESONATOR_H
#define DSY_RESONATOR_H

#include <stdint.h>
#include <stddef.h>
#include "dsp.h"
#ifdef __cplusplus

#define PI_POW_3 PI_F * PI_F * PI_F
#define PI_POW_5 PI_POW_3 * PI_F * PI_F

#define kMaxNumModes 24
#define kModeBatchSize 4

/** @file resonator.h */

namespace daisysp
{

enum FilterMode{
  FILTER_MODE_LOW_PASS,
  FILTER_MODE_BAND_PASS,
  FILTER_MODE_BAND_PASS_NORMALIZED,
  FILTER_MODE_HIGH_PASS
};

// We render 4 modes simultaneously since there are enough registers to hold
// all state variables.
/**  
       SVF for use in the Resonator module \n 
       Ported by Ben Sergentanis, Dec 2020 
       @author Emilie Gillet
       @date 2016
*/
template<int batch_size>
class ResonatorSvf {
 public:
  ResonatorSvf() { }
  ~ResonatorSvf() { }
  
  void Init() {
    for (int i = 0; i < batch_size; ++i) {
      state_1_[i] = state_2_[i] = 0.0f;
    }
  }
  
  template<FilterMode mode, bool add>
  void Process(
      const float* f,
      const float* q,
      const float* gain,
      const float* in,
      float* out,
      size_t size) {
    float g[batch_size];
    float r[batch_size];
    float r_plus_g[batch_size];
    float h[batch_size];
    float state_1[batch_size];
    float state_2[batch_size];
    float gains[batch_size];
    for (int i = 0; i < batch_size; ++i) {
      g[i] = fasttan(f[i]);
      r[i] = 1.0f / q[i];
      h[i] = 1.0f / (1.0f + r[i] * g[i] + g[i] * g[i]);
      r_plus_g[i] = r[i] + g[i];
      state_1[i] = state_1_[i];
      state_2[i] = state_2_[i];
      gains[i] = gain[i];
    }
    
    while (size--) {
      float s_in = *in++;
      float s_out = 0.0f;
      for (int i = 0; i < batch_size; ++i) {
        const float hp = (s_in - r_plus_g[i] * state_1[i] - state_2[i]) * h[i];
        const float bp = g[i] * hp + state_1[i];
        state_1[i] = g[i] * hp + bp;
        const float lp = g[i] * bp + state_2[i];
        state_2[i] = g[i] * bp + lp;
        s_out += gains[i] * ((mode == FILTER_MODE_LOW_PASS) ? lp : bp);
      }
      if (add) {
        *out++ += s_out;
      } else {
        *out++ = s_out;
      }
    }
    for (int i = 0; i < batch_size; ++i) {
      state_1_[i] = state_1[i];
      state_2_[i] = state_2[i];
    }
  }
  
 private:
	static inline float fasttan(float f){
	  const float a = 3.260e-01 * PI_POW_3;
      const float b = 1.823e-01 * PI_POW_5;
      float f2 = f * f;
      return f * (PI_F + f2 * (a + b * f2));
	}

  float state_1_[batch_size];
  float state_2_[batch_size];
};


/**  
       Resonant body simulation \n 
       Ported by Ben Sergentanis, Dec 2020 
       @author Emilie Gillet
       @date 2016
*/
class Resonator {
 public:
  Resonator() { }
  ~Resonator() { }
  
  void Init(float position, int resolution);
  void Process(
      float f0,
      float structure,
      float brightness,
      float damping,
      const float* in,
      float* out,
      size_t size);
  
 private:
  int resolution_;
  
  float mode_amplitude_[kMaxNumModes];
  ResonatorSvf<kModeBatchSize> mode_filters_[kMaxNumModes / kModeBatchSize];
};

}  // namespace daisysp
#endif
#endif