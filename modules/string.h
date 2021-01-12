#pragma once
#ifndef DSY_STRING_H
#define DSY_STRING_H

#include <stdint.h>
#ifdef __cplusplus

/** @file string.h */

namespace daisysp
{

/**  
       @brief Comb filter / KS string.
	   @author Ben Sergentanis
	   @date Jan 2021 
	   "Lite" version of the implementation used in Rings \n \n 
	   Ported from pichenettes/eurorack/plaits/dsp/oscillator/formant_oscillator.h \n
	   to an independent module. \n
	   Original code written by Emilie Gillet in 2016. \n
*/
namespace daisysp {

const size_t kDelayLineSize = 1024;

enum StringNonLinearity {
  STRING_NON_LINEARITY_CURVED_BRIDGE,
  STRING_NON_LINEARITY_DISPERSION
};

class String {
 public:
  String() { }
  ~String() { }
  
  void Init(stmlib::BufferAllocator* allocator);
  void Reset();
  void Process(
      float f0,
      float non_linearity_amount,
      float brightness,
      float damping,
      const float* in,
      float* out,
      size_t size);

 private:
  template<StringNonLinearity non_linearity>
  void ProcessInternal(
      float f0,
      float non_linearity_amount,
      float brightness,
      float damping,
      const float* in,
      float* out,
      size_t size);
  
  DelayLine<float, kDelayLineSize> string_;
  DelayLine<float, kDelayLineSize / 4> stretch_;
  
  stmlib::Svf iir_damping_filter_;
  stmlib::DCBlocker dc_blocker_;
  
  float delay_;
  float dispersion_noise_;
  float curved_bridge_;
  
  // Very crappy linear interpolation upsampler used for low pitches that
  // do not fit the delay line. Rarely used.
  float src_phase_;
  float out_sample_[2];
};
}  // namespace daisysp
#endif
#endif