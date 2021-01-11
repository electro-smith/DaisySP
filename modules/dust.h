#pragma once
#ifndef DSY_DUST_H
#define DSY_DUST_H

#include <stdint.h>
#ifdef __cplusplus

/** @file dust.h */

namespace daisysp
{
/**  
		Randomly Clocked Samples \n
       Ported by Ben Sergentanis, Dec 2020 
       @author Emilie Gillet
       @date 2016
*/
class Dust
{
  public:
    Dust() {}
    ~Dust() {}

	void Init(float sample_rate){
		sample_rate_ = sample_rate;
		SetFreq(1000.f);
	}

    float Process()
    {
        float inv_frequency = 1.0f / frequency_;
        float u             = GetFloat();
        if(u < frequency_)
        {
            return u * inv_frequency;
        }
        return 0.0f;
    }

	void SetFreq(float freq){
		freq /= sample_rate_;
		frequency_ = fclamp(freq, 0.f, 1.f);
	}

  private:
	float frequency_;
	float sample_rate_;
	
    inline float GetFloat()
	{
		rng_state_ = rng_state_ * 1664525L + 1013904223L;
		return static_cast<float>(rng_state_) / 4294967296.0f;
	}
    int32_t      rng_state_ = 0x8D5A61A4; //default seed
};
} // namespace daisysp
#endif
#endif