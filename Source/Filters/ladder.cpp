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
// v1.6 (Infrasonic/Daisy) removes polyphase FIR, uses 2x linear
//      oversampling for performance reasons
// v1.5 adds polyphase FIR or Linear interpolation
// v1.4 FC extended to 18.7kHz, max res to 1.8, 4x oversampling,
//      and a minor Q-tuning adjustment
// v.1.03 adds oversampling, extended resonance,
// and exposes parameters input_drive and passband_gain
// v.1.02 now includes both cutoff and resonance "CV" modulation inputs
// please retain this header if you use this code.
//-----------------------------------------------------------

#include "ladder.h"
#include "Utility/dsp.h"

using namespace daisysp;

static inline float fast_tanh(float x)
{
    if(x > 3.0f)
        return 1.0f;
    if(x < -3.0f)
        return -1.0f;
    float x2 = x * x;
    return x * (27.0f + x2) / (27.0f + 9.0f * x2);
}

void LadderFilter::Init(float sample_rate)
{
    sample_rate_  = sample_rate;
    sr_int_recip_ = 1.0f / (sample_rate * kInterpolation);
    alpha_        = 1.0f;
    K_            = 1.0f;
    Fbase_        = 1000.0f;
    Qadjust_      = 1.0f;
    oldinput_     = 0.f;
    mode_         = FilterMode::LP24;

    SetPassbandGain(0.5f);
    SetInputDrive(0.5f);
    SetFreq(5000.f);
    SetRes(0.2f);
}

float LadderFilter::Process(float in)
{
    float input  = in * drive_;
    float total  = 0.0f;
    float interp = 0.0f;
    for(size_t os = 0; os < kInterpolation; os++)
    {
        float u = (interp * oldinput_ + (1.0f - interp) * input)
                  - (z1_[3] - pbg_ * input) * K_ * Qadjust_;
        u            = fast_tanh(u);
        float stage1 = LPF(u, 0);
        float stage2 = LPF(stage1, 1);
        float stage3 = LPF(stage2, 2);
        float stage4 = LPF(stage3, 3);
        total += weightedSumForCurrentMode(
                     {input, stage1, stage2, stage3, stage4})
                 * kInterpolationRecip;
        interp += kInterpolationRecip;
    }
    oldinput_ = input;
    return total;
}

__attribute__((optimize("unroll-loops"))) void
LadderFilter::ProcessBlock(float* buf, size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        buf[i] = Process(buf[i]);
    }
}

void LadderFilter::SetFreq(float freq)
{
    Fbase_ = freq;
    compute_coeffs(freq);
}

void LadderFilter::SetRes(float res)
{
    // maps resonance = 0->1 to K = 0 -> 4
    res = daisysp::fclamp(res, 0.0f, kMaxResonance);
    K_  = 4.0f * res;
}

void LadderFilter::SetPassbandGain(float pbg)
{
    pbg_ = daisysp::fclamp(pbg, 0.0f, 0.5f);
    SetInputDrive(drive_);
}

void LadderFilter::SetInputDrive(float odrv)
{
    drive_ = daisysp::fmax(odrv, 0.0f);
    if(drive_ > 1.0f)
    {
        drive_ = fmin(drive_, 4.0f);
        // max is 4 when pbg = 0, and 2.5 when pbg is 0.5
        drive_scaled_ = 1.0f + (drive_ - 1.0f) * (1.0f - pbg_);
    }
    else
    {
        drive_scaled_ = drive_;
    }
}

float LadderFilter::LPF(float s, int i)
{
    //             (1.0 / 1.3)   (0.3 / 1.3)
    float ft = s * 0.76923077f + 0.23076923f * z0_[i] - z1_[i];
    ft       = ft * alpha_ + z1_[i];
    z1_[i]   = ft;
    z0_[i]   = s;
    return ft;
}

void LadderFilter::compute_coeffs(float freq)
{
    freq      = daisysp::fclamp(freq, 5.0f, sample_rate_ * 0.425f);
    float wc  = freq * 2.0f * PI_F * sr_int_recip_;
    float wc2 = wc * wc;
    alpha_    = 0.9892f * wc - 0.4324f * wc2 + 0.1381f * wc * wc2
             - 0.0202f * wc2 * wc2;
    //Qadjust = 1.0029f + 0.0526f * wc - 0.0926 * wc2 + 0.0218* wc * wc2;
    Qadjust_ = 1.006f + 0.0536f * wc - 0.095f * wc2 - 0.05f * wc2 * wc2;
    // revised hfQ (rvh - feb 14 2021)
}

float LadderFilter::weightedSumForCurrentMode(
    const std::array<float, 5>& stage_outs)
{
    // Weighted filter stage mixing to achieve selected response
    // as described in "Oscillator and Filter Algorithms for Virtual Analog Synthesis"
    // Välimäki and Huovilainen, Computer Music Journal, vol 60, 2006
    switch(mode_)
    {
        case FilterMode::LP24: return stage_outs[4];
        case FilterMode::LP12: return stage_outs[2];
        case FilterMode::BP24:
            return (stage_outs[2] + stage_outs[4]) * 4.0f
                   - stage_outs[3] * 8.0f;
        case FilterMode::BP12: return (stage_outs[1] - stage_outs[2]) * 2.0f;
        case FilterMode::HP24:
            return stage_outs[0] + stage_outs[4]
                   - ((stage_outs[1] + stage_outs[3]) * 4.0f)
                   + stage_outs[2] * 6.0f;
        case FilterMode::HP12:
            return stage_outs[0] + stage_outs[2] - stage_outs[1] * 2.0f;
        default: return 0.0f;
    }
}
