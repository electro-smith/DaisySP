#pragma once
#ifndef DSY_SYNTHSD_H
#define DSY_SYNTHSD_H

#include "modules/svf.h"

#include <stdint.h>
#ifdef __cplusplus

/** @file synthsnaredrum.h */

namespace daisysp
{
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

    float Process(bool trigger);

	void SetSustain(bool  sustain);
    void SetAccent(float accent);
    void SetFreq(float f0);
    void SetFmAmount(float fm_amount);
    void SetDecay(float decay);
    void SetSnappy(float snappy);

  private:
    float sample_rate_;

	bool sustain_;
	float accent_, f0_, fm_amount_, decay_, snappy_;

    float phase_[2];
    float drum_amplitude_;
    float snare_amplitude_;
    float fm_;
    float sustain_gain_;
    int   hold_counter_;

    float rand_frac_  = 1.f / (float)RAND_MAX;
    float ratio_frac_ = 1.f / 12.f;

    Svf drum_lp_;
    Svf snare_hp_;
    Svf snare_lp_;
};
} // namespace daisysp
#endif
#endif