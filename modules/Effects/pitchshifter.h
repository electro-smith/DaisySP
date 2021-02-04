#pragma once
#ifndef DSY_PITCHSHIFTER_H
#define DSY_PITCHSHIFTER_H
#include <stdint.h>
#include <cmath>
#ifdef USE_ARM_DSP
#include "arm_math.h"
#endif
#include "Utility/dsp.h"
#include "Utility/delayline.h"
#include "Control/phasor.h"

/** Shift can be 30-100 ms lets just start with 50 for now.
0.050 * SR = 2400 samples (at 48kHz)
*/
#define SHIFT_BUFFER_SIZE 16384
//#define SHIFT_BUFFER_SIZE 4800
//#define SHIFT_BUFFER_SIZE 8192
//#define SHIFT_BUFFER_SIZE 1024

namespace daisysp
{
static inline uint32_t hash_xs32(uint32_t x)
{
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return x;
}

inline uint32_t myrand()
{
    static uint32_t seed = 1;
    seed                 = hash_xs32(seed);
    return seed;
}

/**  time-domain pitchshifter

Author: shensley

Based on "Pitch Shifting" from ucsd.edu 

t = 1 - ((s *f) / R)

where:
    s is the size of the delay
    f is the frequency of the lfo
    r is the sample_rate

solving for t = 12.0
f = (12 - 1) * 48000 / SHIFT_BUFFER_SIZE;

\todo - move hash_xs32 and myrand to dsp.h and give appropriate names
*/
class PitchShifter
{
  public:
    PitchShifter() {}
    ~PitchShifter() {}
    /** Initialize pitch shifter
    */
    void Init(float sr)
    {
        force_recalc_ = false;
        sr_           = sr;
        mod_freq_     = 5.0f;
        SetSemitones();
        for(uint8_t i = 0; i < 2; i++)
        {
            gain_[i] = 0.0f;
            d_[i].Init();
            phs_[i].Init(sr, 50, i == 0 ? 0 : PI_F);
        }
        shift_up_ = true;
        del_size_ = SHIFT_BUFFER_SIZE;
        SetDelSize(del_size_);
        fun_ = 0.0f;
    }

    /** process pitch shifter
    */
    float Process(float &in)
    {
        float val, fade1, fade2;
        // First Process delay mod/crossfade
        fade1 = phs_[0].Process();
        fade2 = phs_[1].Process();
        if(prev_phs_a_ > fade1)
        {
            mod_a_amt_ = fun_ * ((float)(myrand() % 255) / 255.0f)
                         * (del_size_ * 0.5f);
            mod_coeff_[0]
                = 0.0002f + (((float)(myrand() % 255) / 255.0f) * 0.001f);
        }
        if(prev_phs_b_ > fade2)
        {
            mod_b_amt_ = fun_ * ((float)(myrand() % 255) / 255.0f)
                         * (del_size_ * 0.5f);
            mod_coeff_[1]
                = 0.0002f + (((float)(myrand() % 255) / 255.0f) * 0.001f);
        }
        slewed_mod_[0] += mod_coeff_[0] * (mod_a_amt_ - slewed_mod_[0]);
        slewed_mod_[1] += mod_coeff_[1] * (mod_b_amt_ - slewed_mod_[1]);
        prev_phs_a_ = fade1;
        prev_phs_b_ = fade2;
        if(shift_up_)
        {
            fade1 = 1.0f - fade1;
            fade2 = 1.0f - fade2;
        }
        mod_[0] = fade1 * (del_size_ - 1);
        mod_[1] = fade2 * (del_size_ - 1);
#ifdef USE_ARM_DSP
        gain_[0] = arm_sin_f32(fade1 * (float)M_PI);
        gain_[1] = arm_sin_f32(fade2 * (float)M_PI);
#else
        gain_[0] = sinf(fade1 * PI_F);
        gain_[1] = sinf(fade2 * PI_F);
#endif

        // Handle Delay Writing
        d_[0].Write(in);
        d_[1].Write(in);
        // Modulate Delay Lines
        //mod_a_amt = mod_b_amt = 0.0f;
        d_[0].SetDelay(mod_[0] + mod_a_amt_);
        d_[1].SetDelay(mod_[1] + mod_b_amt_);
        d_[0].SetDelay(mod_[0] + slewed_mod_[0]);
        d_[1].SetDelay(mod_[1] + slewed_mod_[1]);
        val = 0.0f;
        val += (d_[0].Read() * gain_[0]);
        val += (d_[1].Read() * gain_[1]);
        return val;
    }

    /** sets transposition in semitones
    */
    void SetTransposition(const float &transpose)
    {
        float   ratio;
        uint8_t idx;
        if(transpose_ != transpose || force_recalc_)
        {
            transpose_ = transpose;
            idx        = (uint8_t)fabsf(transpose);
            ratio      = semitone_ratios_[idx % 12];
            ratio *= (uint8_t)(fabsf(transpose) / 12) + 1;
            if(transpose > 0.0f)
            {
                shift_up_ = true;
            }
            else
            {
                shift_up_ = false;
            }
            mod_freq_ = ((ratio - 1.0f) * sr_) / del_size_;
            if(mod_freq_ < 0.0f)
            {
                mod_freq_ = 0.0f;
            }
            phs_[0].SetFreq(mod_freq_);
            phs_[1].SetFreq(mod_freq_);
            if(force_recalc_)
            {
                force_recalc_ = false;
            }
        }
    }

    /** sets delay size changing the timbre of the pitchshifting 
    */
    void SetDelSize(uint32_t size)
    {
        del_size_     = size < SHIFT_BUFFER_SIZE ? size : SHIFT_BUFFER_SIZE;
        force_recalc_ = true;
        SetTransposition(transpose_);
    }

    /** sets an amount of internal random modulation, kind of sounds like tape-flutter
    */
    inline void SetFun(float f) { fun_ = f; }

  private:
    inline void SetSemitones()
    {
        for(size_t i = 0; i < 12; i++)
        {
            semitone_ratios_[i] = powf(2.0f, (float)i / 12);
        }
    }
    typedef DelayLine<float, SHIFT_BUFFER_SIZE> ShiftDelay;
    ShiftDelay                                  d_[2];
    float                                       pitch_shift_, mod_freq_;
    uint32_t                                    del_size_;
    /** lfo stuff
*/
    bool   force_recalc_;
    float  sr_;
    bool   shift_up_;
    Phasor phs_[2];
    float  gain_[2], mod_[2], transpose_;
    float  fun_, mod_a_amt_, mod_b_amt_, prev_phs_a_, prev_phs_b_;
    float  slewed_mod_[2], mod_coeff_[2];
    /** pitch stuff
*/
    float semitone_ratios_[12];
};
} // namespace daisysp

#endif
