#pragma once
#ifndef DSY_SYNTHSD_H
#define DSY_SYNTHSD_H

#include "Filters/svf.h"

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

    /** Init the module
        \param sample_rate Audio engine sample rate
    */
    void Init(float sample_rate);

    /** Get the next sample.
        \param trigger True = hit the drum. This argument is optional.
    */
    float Process(bool trigger = false);

    /** Trigger the drum */
    void Trig();

    /** Make the drum ring out infinitely.
        \param sustain True = infinite sustain.
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

    /** Set the amount of fm sweep.
        \param fm_amount Works from 0 - 1.
    */
    void SetFmAmount(float fm_amount);

    /** Set the length of the drum decay
        \param decay Works with positive numbers
    */
    void SetDecay(float decay);

    /** Sets the mix between snare and drum.
        \param snappy 1 = just snare. 0 = just drum.
    */
    void SetSnappy(float snappy);

  private:
    inline float DistortedSine(float phase);

    float sample_rate_;

    bool  trig_;
    bool  sustain_;
    float accent_, f0_, fm_amount_, decay_, snappy_;

    float phase_[2];
    float drum_amplitude_;
    float snare_amplitude_;
    float fm_;
    float sustain_gain_;
    int   hold_counter_;

    Svf drum_lp_;
    Svf snare_hp_;
    Svf snare_lp_;
};
} // namespace daisysp
#endif
#endif