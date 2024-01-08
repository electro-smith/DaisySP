/*
Copyright (c) 2020 Electrosmith, Corp, Emilie Gillet

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/

#pragma once
#ifndef DSY_ANALOG_SNARE_H
#define DSY_ANALOG_SNARE_H

#include "Filters/svf.h"

#include <stdint.h>
#ifdef __cplusplus

/** @file analogsnaredrum.h */

namespace daisysp
{
/**  
       @brief 808 snare drum model, revisited.
       @author Ben Sergentanis
       @date Jan 2021
       Ported from pichenettes/eurorack/plaits/dsp/drums/analog_snare_drum.h \n
       to an independent module. \n
       Original code written by Emilie Gillet in 2016. \n
*/
class AnalogSnareDrum
{
  public:
    AnalogSnareDrum() {}
    ~AnalogSnareDrum() {}

    static const int kNumModes = 5;

    /** Init the module
        \param sample_rate Audio engine sample rate
    */
    void Init(float sample_rate);

    /** Get the next sample
        \param trigger Hit the drum with true. Defaults to false.
    */
    float Process(bool trigger = false);

    /** Trigger the drum */
    void Trig();

    /** Init the module
        \param sample_rate Audio engine sample rate
    */
    void SetSustain(bool sustain);

    /** Set how much accent to use
        \param accent Works 0-1.
    */
    void SetAccent(float accent);

    /** Set the drum's root frequency
        \param f0 Freq in Hz
    */
    void SetFreq(float f0);

    /** Set the brightness of the drum tone.
        \param tone Works 0-1. 1 = bright, 0 = dark.
    */
    void SetTone(float tone);

    /** Set the length of the drum decay
        \param decay Works with positive numbers
    */
    void SetDecay(float decay);

    /** Sets the mix between snare and drum.
        \param snappy 1 = just snare. 0 = just drum.
    */
    void SetSnappy(float snappy);

  private:
    float sample_rate_;

    float f0_, tone_, accent_, snappy_, decay_;
    bool  sustain_;
    bool  trig_;

    inline float SoftLimit(float x);
    inline float SoftClip(float x);

    int   pulse_remaining_samples_;
    float pulse_;
    float pulse_height_;
    float pulse_lp_;
    float noise_envelope_;
    float sustain_gain_;

    Svf resonator_[kNumModes];
    Svf noise_filter_;

    // Replace the resonators in "free running" (sustain) mode.
    float phase_[kNumModes];
};
} // namespace daisysp
#endif
#endif
