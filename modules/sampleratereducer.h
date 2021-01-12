#pragma once
#ifndef DSY_SR_REDUCER_H
#define DSY_SR_REDUCER_H

#include <stdint.h>
#ifdef __cplusplus

/** @file sampleratereducer.h */

namespace daisysp
{
/** 
       @brief Sample rate reducer.
	   @author Ben Sergentanis
	   @date Jan 2021
	   Ported from pichenettes/eurorack/plaits/dsp/fx/sample_rate_reducer.h \n
	   to an independent module. \n
	   Original code written by Emilie Gillet in 2014. \n
*/
class SampleRateReducer {
 public:
  SampleRateReducer() { }
  ~SampleRateReducer() { }
  
  void Init();
  
  float Process(float frequency, float in);
  
 private:
  float phase_;
  float sample_;
  float previous_sample_;
  float next_sample_;
  };
}  // namespace daisysp
#endif
#endif