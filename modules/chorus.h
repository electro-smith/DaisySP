#pragma once
#ifndef DSY_CHORUS_H
#define DSY_CHORUS_H
#ifdef __cplusplus

#include <stdint.h>
#include "modules/delayline.h"

#define NUM_DEL 2
#define DEL_LEN 48000

/** @file chorus.h */

namespace daisysp
{
/**  
    @brief Chorus Effect.
	@author Ben Sergentanis
	@date Jan 2021
	Based on https://www.izotope.com/en/learn/understanding-chorus-flangers-and-phasers-in-audio-production.html \n
	and https://www.researchgate.net/publication/236629475_Implementing_Professional_Audio_Effects_with_DSPs \n
*/
class Chorus
{
  public:
    Chorus() {}
    ~Chorus() {}

    void Init(float sample_rate);

    float Process(float in);

    /** How much +/- the lfo will move its delayline in ms. */
    void SetLfoDepth(float depth, int idx = 0);

    /** How fast the lfo will cycle in Hz */
    void SetLfoFreq(float freq, int idx = 0);

    /** This delayline's base delay in ms */
    void SetDelay(float ms, int idx = 0);

    void SetLfoDepthAll(float depth);

    /** How fast the lfo will cycle in Hz */
    void SetLfoFreqAll(float freq);

    /** This delayline's base delay in ms */
    void SetDelayAll(float ms);

  private:
    float sample_rate_;

    //triangle lfos
    float ProcessLfo(int idx);

    float lfo_phase_[NUM_DEL];
    float lfo_freq_[NUM_DEL];
    float lfo_amp_[NUM_DEL];

    float delay_[NUM_DEL];
    float pre_gain_;

    DelayLine<float, DEL_LEN> del_[NUM_DEL];
};
} //namespace daisysp
#endif
#endif