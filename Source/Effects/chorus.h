#pragma once
#ifndef DSY_CHORUS_H
#define DSY_CHORUS_H
#ifdef __cplusplus

#include <stdint.h>
#include "Utility/delayline.h"

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
        \param ms Delay time in ms, 0 to 50 ms.
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

    /** Initialize the module
        \param sample_rate Audio engine sample rate
    */
    void Init(float sample_rate);

    /** Get the net floating point sample. Defaults to left channel.
        \param in Sample to process
    */
    float Process(float in);

    /** Get the left channel's last sample */
    float GetLeft();

    /** Get the right channel's last sample */
    float GetRight();

    /** Pan both channels individually.
        \param panl Pan the left channel. 0 is left, 1 is right.
        \param panr Pan the right channel.
    */
    void SetPan(float panl, float panr);

    /** Pan both channels.
        \param pan Where to pan both channels to. 0 is left, 1 is right.
    */
    void SetPan(float pan);

    /** Set both lfo depths individually.
        \param depthl Left channel lfo depth. Works 0-1.
        \param depthr Right channel lfo depth.
    */
    void SetLfoDepth(float depthl, float depthr);

    /** Set both lfo depths.
        \param depth Both channels lfo depth. Works 0-1.
    */
    void SetLfoDepth(float depth);

    /** Set both lfo frequencies individually.
        \param depthl Left channel lfo freq in Hz.
        \param depthr Right channel lfo freq in Hz.
    */
    void SetLfoFreq(float freql, float freqr);

    /** Set both lfo frequencies.
        \param depth Both channel lfo freqs in Hz.
    */
    void SetLfoFreq(float freq);

    /** Set both channel delay amounts individually.
        \param delayl Left channel delay amount. Works 0-1.
        \param delayr Right channel delay amount.
    */
    void SetDelay(float delayl, float delayr);

    /** Set both channel delay amounts.
        \param delay Both channel delay amount. Works 0-1.
    */
    void SetDelay(float delay);

    /** Set both channel delay individually.
        \param msl Left channel delay in ms.
        \param msr Right channel delay in ms.
    */
    void SetDelayMs(float msl, float msr);

    /** Set both channel delay in ms.
        \param ms Both channel delay amounts in ms.
    */
    void SetDelayMs(float ms);

    /** Set both channels feedback individually.
        \param feedbackl Left channel feedback. Works 0-1.
        \param feedbackr Right channel feedback.
    */
    void SetFeedback(float feedbackl, float feedbackr);

    /** Set both channels feedback.
        \param feedback Both channel feedback. Works 0-1.
    */
    void SetFeedback(float feedback);

  private:
    ChorusEngine engines_[2];
    float        gain_frac_;
    float        pan_[2];

    float sigl_, sigr_;
};
} //namespace daisysp
#endif
#endif