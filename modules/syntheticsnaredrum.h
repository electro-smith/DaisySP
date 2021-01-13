#pragma once
#ifndef DSY_SYNTHSD_H
#define DSY_SYNTHSD_H

#include "modules/svf.h"

#include <stdint.h>
#ifdef __cplusplus

/** @file syntheticsnaredrum.h */

namespace daisysp{

    /**  
       @brief Naive snare drum model (two modulated oscillators + filtered noise).
	   @author Ben Sergentanis
	   @date Jan 2021
       Uses a few magic numbers taken from the 909 schematics: \n 
       - Ratio between the two modes of the drum set to 1.47. \n
       - Funky coupling between the two modes. \n
       - Noise coloration filters and envelope shapes for the snare. \n \n
	   Ported from pichenettes/eurorack/plaits/dsp/drums/synthetic_snare_drum.h \n
	   to an independent module. \n
	   Original code written by Emilie Gillet in 2016. \n
*/
    class SyntheticSnareDrum
{
  public:
    SyntheticSnareDrum() {}
    ~SyntheticSnareDrum() {}

    void Init(float sample_rate);

    inline float DistortedSine(float phase);
	
    void Process(bool   sustain,
                bool   trigger,
                float  accent,
                float  f0,
                float  fm_amount,
                float  decay,
                float  snappy,
                float* out,
                size_t size);

  private:
	float sample_rate_;
  
    float phase_[2];
    float drum_amplitude_;
    float snare_amplitude_;
    float fm_;
    float sustain_gain_;
    int   hold_counter_;

	float rand_frac_ = 1.f / (float)RAND_MAX;
	float ratio_frac_ = 1.f / 12.f;
	
    Svf drum_lp_;
    Svf snare_hp_;
    Svf     snare_lp_;
};
} // namespace daisysp
#endif
#endif