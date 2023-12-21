/*
Copyright (c) 2020 Electrosmith, Corp

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/

#pragma once
#ifndef DSY_PHASER_H
#define DSY_PHASER_H
#ifdef __cplusplus

#include <stdint.h>
#include "Utility/delayline.h"

/** @file phaser.h */

namespace daisysp
{
/**  
    @brief Single Phaser engine. Used in Phaser.
    @author Ben Sergentanis
*/
class PhaserEngine
{
  public:
    PhaserEngine() {}
    ~PhaserEngine() {}

    /** Initialize the module
        \param sample_rate Audio engine sample rate.
    */
    void Init(float sample_rate);

    /** Get the next sample
        \param in Sample to process
    */
    float Process(float in);

    /** How much to modulate the allpass filter by.
        \param depth Works 0-1.
    */
    void SetLfoDepth(float depth);

    /** Set lfo frequency.
        \param lfo_freq Frequency in Hz
    */
    void SetLfoFreq(float lfo_freq);

    /** Set the allpass frequency
        \param ap_freq Frequency in Hz.
    */
    void SetFreq(float ap_freq);

    /** Set the feedback amount.
        \param feedback Amount from 0-1.
    */
    void SetFeedback(float feedback);

  private:
    float                    sample_rate_;
    static constexpr int32_t kDelayLength
        = 2400; // 50 ms at 48kHz = .05 * 48000

    //triangle lfo
    float lfo_phase_;
    float lfo_freq_;
    float lfo_amp_;

    float os_;

    float feedback_;
    float ap_freq_;

    float deltime_;
    float last_sample_;

    DelayLine<float, kDelayLength> del_;

    float ProcessLfo();
};

//wraps up all of the phaser engines
/**  
    @brief Phaser Effect.
    @author Ben Sergentanis
    @date Jan 2021
*/
class Phaser
{
  public:
    Phaser() {}
    ~Phaser() {}

    /** Initialize the module
        \param sample_rate Audio engine sample rate
    */
    void Init(float sample_rate);

    /** Get the next floating point sample.
        \param in Sample to process
    */
    float Process(float in);

    /** Number of allpass stages.
        \param poles Works 1 to 8.
    */
    void SetPoles(int poles);

    /** Set all lfo depths
        \param depth Works 0-1.
    */
    void SetLfoDepth(float depth);

    /** Set all lfo frequencies.
        \param lfo_freq Lfo freq in Hz.
    */
    void SetLfoFreq(float lfo_freq);

    /** Set all channel allpass freq in Hz.
        \param ap_freq Frequency in Hz.
    */
    void SetFreq(float ap_freq);

    /** Set all channels feedback.
        \param feedback Works 0-1.
    */
    void SetFeedback(float feedback);

  private:
    static constexpr int kMaxPoles = 8;
    PhaserEngine         engines_[kMaxPoles];
    float                gain_frac_;
    int                  poles_;
};
} //namespace daisysp
#endif
#endif
