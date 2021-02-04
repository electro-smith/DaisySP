#pragma once
#ifndef DSY_ANALOG_BD_H
#define DSY_ANALOG_BD_H

#include <stdint.h>
#ifdef __cplusplus

#include "Synthesis/oscillator.h"
#include "Filters/svf.h"

/** @file analogbassdrum.h */

namespace daisysp
{
/**  
       @brief 808 bass drum model, revisited.
       @author Ben Sergentanis
       @date Jan 2021 
       Ported from pichenettes/eurorack/plaits/dsp/drums/analog_bass_drum.h \n
       to an independent module. \n
       Original code written by Emilie Gillet in 2016. \n
*/
class AnalogBassDrum
{
  public:
    AnalogBassDrum() {}
    ~AnalogBassDrum() {}

    /** Initialize the module
        \param sample_rate Audio engine sample rate
    */
    void Init(float sample_rate);

    /** Get the next sample.
        \param trigger True strikes the drum. Defaults to false.
    */
    float Process(bool trigger = false);

    /** Strikes the drum. */
    void Trig();

    /** Set the bassdrum to play infinitely
        \param sustain True = infinite length
    */
    void SetSustain(bool sustain);

    /** Set a small accent.
        \param accent Works 0-1
    */
    void SetAccent(float accent);

    /** Set the drum's root frequency
        \param f0 Frequency in Hz
    */
    void SetFreq(float f0);

    /** Set the amount of click.
        \param tone Works 0-1.
    */
    void SetTone(float tone);

    /** Set the decay length of the drum.
        \param decay Works best 0-1.
    */
    void SetDecay(float decay);

    /** Set the amount of fm attack. Works together with self fm.
        \param attack_fm_amount Works best 0-1.
    */
    void SetAttackFmAmount(float attack_fm_amount);

    /**Set the amount of self fm. Also affects fm attack, and volume decay.
        \param self_fm_amount Works best 0-1.
    */
    void SetSelfFmAmount(float self_fm_amount);

  private:
    inline float Diode(float x);

    float sample_rate_;

    float accent_, f0_, tone_, decay_;
    float attack_fm_amount_, self_fm_amount_;

    bool trig_, sustain_;

    int   pulse_remaining_samples_;
    int   fm_pulse_remaining_samples_;
    float pulse_;
    float pulse_height_;
    float pulse_lp_;
    float fm_pulse_lp_;
    float retrig_pulse_;
    float lp_out_;
    float tone_lp_;
    float sustain_gain_;

    Svf resonator_;

    //for use in sin + cos osc. in sustain mode
    float phase_;
};
} // namespace daisysp
#endif
#endif