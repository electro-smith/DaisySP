#pragma once
#ifndef DSY_SYNTHSD_H
#define DSY_SYNTHSD_H

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

    void Init();

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
    float phase_[2];
    float drum_amplitude_;
    float snare_amplitude_;
    float fm_;
    float sustain_gain_;
    int   hold_counter_;

    stmlib::OnePole drum_lp_;
    stmlib::OnePole snare_hp_;
    stmlib::Svf     snare_lp_;
};
} // namespace daisysp
#endif
#endif