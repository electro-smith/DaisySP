#pragma once
#ifndef DSY_STRING_H
#define DSY_STRING_H

#include <stdint.h>


#include "modules/crossfade.h"
#include "modules/dcblock.h"
#include "modules/delayline.h"
#include "modules/svf.h"

#ifdef __cplusplus

/** @file string.h */

namespace daisysp
{
const size_t kDelayLineSize = 1024;

enum StringNonLinearity
{
    STRING_NON_LINEARITY_CURVED_BRIDGE,
    STRING_NON_LINEARITY_DISPERSION
};

/**  
       @brief Comb filter / KS string.
	   @author Ben Sergentanis
	   @date Jan 2021 
	   "Lite" version of the implementation used in Rings \n \n 
	   Ported from pichenettes/eurorack/plaits/dsp/oscillator/formant_oscillator.h \n
	   to an independent module. \n
	   Original code written by Emilie Gillet in 2016. \n
*/
class String
{
  public:
    String() {}
    ~String() {}

    void Init(float sample_rate);
    void Reset();
    float Process(const float in);
				 
	void SetFreq(float freq);
	void SetNonLinearity(float non_linearity_amount);
	void SetBrightness(float brightness);
	void SetDamping(float damping);
				 

  private:
    template <StringNonLinearity non_linearity>
    float ProcessInternal(const float in);

    DelayLine<float, kDelayLineSize>     string_;
    DelayLine<float, kDelayLineSize / 4> stretch_;

	float frequency_, non_linearity_amount_, brightness_, damping_;

    float sample_rate_;
    float rand_frac_  = 1.f / (float)RAND_MAX;
    float ratio_frac_ = 1.f / 12.f;

    Svf     iir_damping_filter_;
    DcBlock dc_blocker_;

	CrossFade crossfade_;

    float delay_;
    float dispersion_noise_;
    float curved_bridge_;

    // Very crappy linear interpolation upsampler used for low pitches that
    // do not fit the delay line. Rarely used.
    float src_phase_;
    float out_sample_[2];
};
} // namespace daisysp
#endif
#endif