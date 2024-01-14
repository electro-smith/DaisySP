/* Ported from Audio Library for Teensy, Ladder Filter
 * Copyright (c) 2021, Richard van Hoesel
 * Copyright (c) 2024, Infrasonic Audio LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

//-----------------------------------------------------------
// Huovilainen New Moog (HNM) model as per CMJ jun 2006
// Richard van Hoesel, v. 1.03, Feb. 14 2021
// v1.7 (Infrasonic/Daisy) add configurable filter mode
// v1.6 (Infrasonic/Daisy) removes polyphase FIR, uses 4x linear
//      oversampling for performance reasons
// v1.5 adds polyphase FIR or Linear interpolation
// v1.4 FC extended to 18.7kHz, max res to 1.8, 4x oversampling,
//      and a minor Q-tuning adjustment
// v.1.03 adds oversampling, extended resonance,
// and exposes parameters input_drive and passband_gain
// v.1.02 now includes both cutoff and resonance "CV" modulation inputs
// please retain this header if you use this code.
//-----------------------------------------------------------

#pragma once
#ifndef DSY_LADDER_H
#define DSY_LADDER_H

#include <stdlib.h>
#include <stdint.h>
#include <array>
#ifdef __cplusplus

namespace daisysp
{
/**
 * 4-pole ladder filter model with selectable filter type (LP/BP/HP 12 or 24 dB/oct),
 * drive, passband gain compensation, and stable self-oscillation.
 *
 *
 */
class LadderFilter
{
  public:
    enum class FilterMode
    {
        LP24,
        LP12,
        BP24,
        BP12,
        HP24,
        HP12
    };

    LadderFilter()  = default;
    ~LadderFilter() = default;

    /** Initializes the ladder filter module.
     */
    void Init(float sample_rate);

    /** Process single sample */
    float Process(float in);

    /** Process mono buffer/block of samples in place */
    void ProcessBlock(float* buf, size_t size);

    /**
        Sets the cutoff frequency of the filter.
        Units of hz, valid in range 5 - ~nyquist (samp_rate / 2)
        Internally clamped to this range.
    */
    void SetFreq(float freq);

    /**
        Sets the resonance of the filter.
        Filter will stably self oscillate at higher values.
        Valid in range 0 - 1.8
        Internally clamped to this range.
    */
    void SetRes(float res);

    /**
        Set "passband gain" compensation factor to mitigate
        loss of energy in passband at higher resonance values.
        Drive and passband gain have a dependent relationship.
        Valid in range 0 - 0.5
        Internally clamped to this range.
     */
    void SetPassbandGain(float pbg);

    /**
        Sets drive of the input stage into the tanh clipper
        Valid in range 0 - 4.0
     */
    void SetInputDrive(float drv);

    /**
        Sets the filter mode/response
        Defaults to classic lowpass 24dB/oct
     */
    inline void SetFilterMode(FilterMode mode) { mode_ = mode; }

  private:
    static constexpr uint8_t kInterpolation      = 4;
    static constexpr float   kInterpolationRecip = 1.0f / kInterpolation;
    static constexpr float   kMaxResonance       = 1.8f;

    float      sample_rate_, sr_int_recip_;
    float      alpha_;
    float      beta_[4] = {0.0, 0.0, 0.0, 0.0};
    float      z0_[4]   = {0.0, 0.0, 0.0, 0.0};
    float      z1_[4]   = {0.0, 0.0, 0.0, 0.0};
    float      K_;
    float      Fbase_;
    float      Qadjust_;
    float      pbg_;
    float      drive_, drive_scaled_;
    float      oldinput_;
    FilterMode mode_;

    float LPF(float s, int i);
    void  compute_coeffs(float fc);
    float weightedSumForCurrentMode(const std::array<float, 5>& stage_outs);
};


} // namespace daisysp
#endif
#endif
