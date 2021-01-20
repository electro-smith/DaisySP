#pragma once
#ifndef DSY_CHORUS_H
#define DSY_CHORUS_H
#ifdef __cplusplus

#include <stdint.h>
#include "modules/delayline.h"

#define NUM_DEL 1
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
	
	void SetDryWet(float wet);
	
  private:
	float sample_rate_;

	//triangle lfos
	float ProcessLfo(int idx);
	
	float lfo_phase_[NUM_DEL];
	float lfo_freq_[NUM_DEL];
	float lfo_amp_[NUM_DEL];

	float max_del_;
	float pre_gain_;
		
	static DelayLine<float, DEL_LEN> del_[NUM_DEL];
};
} //namespace daisysp
#endif
#endif