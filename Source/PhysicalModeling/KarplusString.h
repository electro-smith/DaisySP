/*
Copyright (c) 2020 Electrosmith, Corp, Emilie Gillet

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/

#pragma once
#ifndef DSY_STRING_H
#define DSY_STRING_H

#include <stdint.h>

#include "Dynamics/crossfade.h"
#include "Utility/dcblock.h"
#include "Utility/delayline.h"
#include "Filters/onepole.h"

#ifdef __cplusplus

/** @file KarplusString.h */

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
class String
{
  public:
    String() {}
    ~String() {}

    /** Initialize the module.
        \param sample_rate Audio engine sample rate
    */
    void Init(float sample_rate);

    /** Clear the delay line */
    void Reset();

    /** Get the next floating point sample
        \param in Signal to excite the string.
    */
    float Process(const float in);

    /** Set the string frequency.
        \param freq Frequency in Hz
    */
    void SetFreq(float freq);

    /** Set the string's behavior.
        \param -1 to 0 is curved bridge, 0 to 1 is dispersion.
    */
    void SetNonLinearity(float non_linearity_amount);

    /** Set the string's overall brightness
        \param Works 0-1.
    */
    void SetBrightness(float brightness);

    /** Set the string's decay time.
        \param damping Works 0-1.
    */
    void SetDamping(float damping);


  private:
    static constexpr size_t kDelayLineSize = 1024;

    enum StringNonLinearity
    {
        NON_LINEARITY_CURVED_BRIDGE,
        NON_LINEARITY_DISPERSION
    };

    template <String::StringNonLinearity non_linearity>
    float ProcessInternal(const float in);

    DelayLine<float, kDelayLineSize>     string_;
    DelayLine<float, kDelayLineSize / 4> stretch_;

    float frequency_, non_linearity_amount_, brightness_, damping_;

    float sample_rate_;

    OnePole iir_damping_filter_;

    DcBlock dc_blocker_;

    CrossFade crossfade_;

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
