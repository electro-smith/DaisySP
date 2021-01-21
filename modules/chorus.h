#pragma once
#ifndef DSY_CHORUS_H
#define DSY_CHORUS_H
#ifdef __cplusplus

#include <stdint.h>
#include "modules/delayline.h"

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
template <int NUM_DEL = 2>
class Chorus
{
  public:
    Chorus() {}
    ~Chorus() {}


    void Init(float sample_rate)
    {
        sample_rate_ = sample_rate;

        pre_gain_ = 1.f / (NUM_DEL + 1);

        for(int i = 0; i < NUM_DEL; i++)
        {
            del_[i].Init();
            lfo_amp_[i] = 0.f;
            SetDelay(2 + 2 * i, i);

            lfo_phase_[i] = 0.f;
            SetLfoFreq(.5 + .05 * i, i);
            SetLfoDepth(2, i);
        }
    }

    float Process(float in)
    {
        float ret = 0.f;

        for(int i = 0; i < NUM_DEL; i++)
        {
            float lfo_sig = ProcessLfo(i);
            del_[i].SetDelay(lfo_sig + delay_[i]);

            ret += del_[i].Read();
            del_[i].Write(in);
        }

        return in * pre_gain_ + ret * pre_gain_;
    }

    void SetLfoDepth(float depth, int idx = 0)
    {
        if(idx < 0 || idx >= NUM_DEL)
        {
            return;
        }

        depth *= sample_rate_ * .001f;
        lfo_amp_[idx] = fclamp(depth, 0.f, delay_[idx]);
    }

    void SetLfoFreq(float freq, int idx = 0)
    {
        if(idx < 0 || idx >= NUM_DEL)
        {
            return;
        }

        freq = 4.f * freq / sample_rate_;
        freq *= lfo_freq_[idx] < 0.f ? -1.f
                                     : 1.f; //if we're headed down, keep going
        lfo_freq_[idx] = fclamp(freq, -.25f, .25f); //clip at +/- .125 * sr
    }

    void SetDelay(float ms, int idx = 0)
    {
        if(idx < 0 || idx >= NUM_DEL)
        {
            return;
        }

        ms *= .001f * sample_rate_;
        delay_[idx] = fmax(ms, 0.0001f * sample_rate_);

        lfo_amp_[idx] = fmin(lfo_amp_[idx], delay_[idx]); //clip this if needed
    }

    void SetLfoDepthAll(float depth)
    {
        for(int i = 0; i < NUM_DEL; i++)
        {
            SetLfoDepth(depth, i);
        }
    }

    void SetLfoFreqAll(float freq)
    {
        for(int i = 0; i < NUM_DEL; i++)
        {
            SetLfoFreq(freq, i);
        }
    }

    void SetDelayAll(float ms)
    {
        for(int i = 0; i < NUM_DEL; i++)
        {
            SetDelay(ms, i);
        }
    }

  private:
    float sample_rate_;

    //triangle lfos
    float lfo_phase_[NUM_DEL];
    float lfo_freq_[NUM_DEL];
    float lfo_amp_[NUM_DEL];

    float delay_[NUM_DEL];
    float pre_gain_;

    DelayLine<float, DEL_LEN> del_[NUM_DEL];

    float ProcessLfo(int idx)
    {
        lfo_phase_[idx] += lfo_freq_[idx];

        //wrap around and flip direction
        if(lfo_phase_[idx] > 1.f)
        {
            lfo_phase_[idx] = 1.f - (lfo_phase_[idx] - 1.f);
            lfo_freq_[idx] *= -1.f;
        }
        else if(lfo_phase_[idx] < -1.f)
        {
            lfo_phase_[idx] = -1.f - (lfo_phase_[idx] + 1.f);
            lfo_freq_[idx] *= -1.f;
        }

        return lfo_phase_[idx] * lfo_amp_[idx];
    }
};
} //namespace daisysp
#endif
#endif