#pragma once
#ifndef DSY_FORMANTOSCILLATOR_H
#define DSY_FORMANTOSCILLATOR_H

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

    void Init();

    void Process(bool   sustain,
                 bool   trigger,
                 float  accent,
                 float  f0,
                 float  tone,
                 float  decay,
                 float  snappy,
                 float* out,
                 size_t size);

  private:
    float sample_rate_;

    int   pulse_remaining_samples_;
    float pulse_;
    float pulse_height_;
    float pulse_lp_;
    float noise_envelope_;
    float sustain_gain_;

    Svf resonator_[kNumModes];
    Svf noise_filter_;

    // Replace the resonators in "free running" (sustain) mode.
    SineOscillator oscillator_[kNumModes];
};
} // namespace daisysp
#endif
#endif