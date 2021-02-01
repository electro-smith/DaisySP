#pragma once
#ifndef DSY_CHORUS_H
#define DSY_CHORUS_H
#ifdef __cplusplus

#include <stdint.h>
#include "modules/delayline.h"

/** @file chorus.h */

namespace daisysp
{
/**  
    @brief Single Chorus engine. Used in Chorus.
	@author Ben Sergentanis
*/
class ChorusEngine
{
  public:
    ChorusEngine() {}
    ~ChorusEngine() {}

    /** Initialize the module
		\param sample_rate Audio engine sample rate.
	*/
    void Init(float sample_rate);

    /** Get the next sample
		\param in Sample to process
	*/
    float Process(float in);

    /** How much to modulate the delay by.
		\param depth Works 0-1.
	*/
    void SetLfoDepth(float depth);

    /** Set lfo frequency.
		\param freq Frequency in Hz
	*/
    void SetLfoFreq(float freq);

    /** Set the internal delay rate. 
		\param delay Tuned for 0-1. Maps to .1 to 50 ms.
	*/
    void SetDelay(float delay);

    /** Set the delay time in ms.
		\param ms Delay time in ms.
	*/
    void SetDelayMs(float ms);

    /** Set the feedback amount.
		\param feedback Amount from 0-1.
	*/
    void SetFeedback(float feedback);

  private:
    float                    sample_rate_;
    static constexpr int32_t kDelayLength
        = 2400; // 50 ms at 48kHz = .05 * 48000

    //triangle lfos
    float lfo_phase_;
    float lfo_freq_;
    float lfo_amp_;

    float feedback_;

    float delay_;

    DelayLine<float, kDelayLength> del_;

    float ProcessLfo();
};

//wraps up all of the chorus engines
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

    float GetLeft();

    float GetRight();

    void SetPan(float panl, float panr = 0.f);

    void SetLfoDepth(float depthl, float depthr = 0.f);

    void SetLfoFreq(float freql, float freqr = 0.f);

    void SetDelay(float delayl, float delayr = 0.f);

    void SetFeedback(float feedbackl, float feedbackr = 0.f);

  private:
    ChorusEngine engines_[2];
    float        gain_frac_;
    float        pan_[2];

    float sigl_, sigr_;
};
} //namespace daisysp
#endif
#endif