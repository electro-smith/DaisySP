#pragma once
#ifndef DSY_CLOCKEDNOISE_H
#define DSY_CLOCKEDNOISE_H

#include <stdint.h>
#ifdef __cplusplus

/** @file clockednoise.h */

namespace daisysp
{
/**  
	   Noise processed by a sample and hold running at a target frequency. \n
       Ported by Ben Sergentanis, Jan 2021 
       @author Emilie Gillet
       @date 2016
*/
class ClockedNoise
{
  public:
    ClockedNoise() {}
    ~ClockedNoise() {}

    /** Initialize module	
		\param sample_rate Audio engine sample rate
	*/
    void Init(float sample_rate);

    /** Get the next floating point sample */
    float Process();

    /** Set the frequency at which the next sample is generated.
		\param freq Frequency in Hz
	*/
    void SetFreq(float freq);

    /** Calling this forces another random float to be generated */
    void Sync();

  private:
    // Oscillator state.
    float phase_;
    float sample_;
    float next_sample_;

    // For interpolation of parameters.
    float frequency_;

    float sample_rate_;

    //from stmlib random.h
    inline float GetFloat();
    int32_t      rng_state_ = 0x8D5A61A4; //default seed
};
} // namespace daisysp
#endif
#endif