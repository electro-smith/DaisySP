#pragma once
#ifndef DSY_ANALOG_BD_H
#define DSY_ANALOG_BD_H

#include <stdint.h>
#ifdef __cplusplus

#include "modules/oscillator.h"
#include "modules/svf.h"

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

    void Init(float sample_rate);

    void Process(bool   sustain,
                 bool   trigger,
                 float  accent,
                 float  f0,
                 float  tone,
                 float  decay,
                 float  attack_fm_amount,
                 float  self_fm_amount,
                 float* out,
                 size_t size);

  private:
    inline float Diode(float x);

    float sample_rate_;

    float ratio_frac_ = 1.f / 12.f;

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