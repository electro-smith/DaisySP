#pragma once
#ifndef DSY_CHORUS_H
#define DSY_CHORUS_H
#ifdef __cplusplus

#include <stdint.h>
#include "modules/delayline.h"

/** @file chorus.h */

namespace daisysp
{
//does the heavy lifting
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

    /** How much of the signal to feedback into the delay line.
		\param feedback Works 0-1.
	*/
    void SetFeedback(float feedback);

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

  private:
    float                    sample_rate_;
    static constexpr int32_t kDelayLength = 2400; // 50 ms at 48kHz = .05 * 48000

    float feedback_;

    //triangle lfos
    float lfo_phase_;
    float lfo_freq_;
    float lfo_amp_;

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
template <int NUM_DELS = 2>
class Chorus
{
  public:
    Chorus() {}
    ~Chorus() {}

    void Init(float sample_rate)
    {
		float f = .5f;
		if(NUM_DELS > 1){
			f = 1.f / (float)(NUM_DELS - 1);
        }
		
		for(int i = 0; i < NUM_DELS; i++)
        {
            engines_[i].Init(sample_rate);
			SetPan(i * f, i);
        }
				
		gain_frac_ = 1.f / (float)NUM_DELS;
    }

	void Process(float in, float& outl, float& outr){
		float l = 0.f;
		float r = 0.f;

		for(int i = 0; i < NUM_DELS; i++){
			float sig = engines_[i].Process(in);
			l += (1.f - pan_[i]) * sig;
			r += pan_[i] * sig;
		}
		
		outl = l * gain_frac_;
		outr = r * gain_frac_;
	}

    float ProcessSingle(float in, int delnum = 0)
    {
        return engines_[delnum].Process(in);
    }
	
	void SetPan(float pan, int delnum = 0){
		pan_[delnum] = fclamp(pan, 0.f, 1.f);;
	}

    void SetLfoDepth(float depth, int delnum = 0)
    {
        engines_[delnum].SetLfoDepth(depth);
    }

    void SetLfoFreq(float freq, int delnum = 0)
    {
        engines_[delnum].SetLfoFreq(freq);
    }

    void SetDelay(float ms, int delnum = 0)
    {
        engines_[delnum].SetDelay(ms);
    }

    void SetLfoDepthAll(float depth)
    {
        for(int i = 0; i < NUM_DELS; i++)
        {
            engines_[i].SetLfoDepth(depth);
        }
    }

    void SetLfoFreqAll(float freq)
    {
        for(int i = 0; i < NUM_DELS; i++)
        {
            engines_[i].SetLfoFreq(freq);
        }
    }

    void SetDelayAll(float ms)
    {
        for(int i = 0; i < NUM_DELS; i++)
        {
            engines_[i].SetDelay(ms);
        }
    }

  private:
    ChorusEngine engines_[NUM_DELS];
	float gain_frac_;
	float pan_[NUM_DELS];
};
} //namespace daisysp
#endif
#endif