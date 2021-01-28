#pragma once
#ifndef DSY_FLANGER_H
#define DSY_FLANGER_H
#ifdef __cplusplus

#include <stdint.h>
#include "modules/delayline.h"

/** @file flanger.h */

namespace daisysp
{
class Flanger
{
  public:
    void Init(float sample_rate);
	
    float Process(float in);
	
    void SetLfoDepth(float depth);
    
	void SetLfoDepthMs(float depth);
	
    void SetLfoFreq(float freq);
	
    void SetDelay(float ms);
	
    void SetDelayMs(float ms);
		
  private:
    float                    sample_rate_;
    static constexpr int32_t kDelayLength = 960; // 20 ms at 48kHz = .02 * 48000

    //triangle lfos
    float lfo_phase_;
    float lfo_freq_;
    float lfo_amp_;

    float delay_;
	
    DelayLine<float, kDelayLength> del_;

    float ProcessLfo();
};
} //namespace daisysp
#endif
#endif