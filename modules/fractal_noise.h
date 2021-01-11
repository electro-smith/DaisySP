#pragma once
#ifndef DSY_FRACTAL_H
#define DSY_FRACTAL_H

#include <stdint.h>
#ifdef __cplusplus

/** @file fractal_noise.h */

namespace daisysp
{
/**  
       Stack octaves of a noise source to generate a fractal noise. \n  
	   T is the noise source to use. T must have SetFreq() and Init(sample_rate) functions. \n
	   Order is the number of noise sources to stack. \n
       Ported by Ben Sergentanis, Jan 2021 
       @author Emilie Gillet
       @date 2016
*/
template <typename T, int order>
class FractalRandomGenerator
{
  public:
    FractalRandomGenerator() {}
    ~FractalRandomGenerator() {}

	/** Initialize the module
		\param sample_rate Audio engine sample rate.
	*/
    void Init(float sample_rate)
    {
		sample_rate_ = sample_rate;
		
        SetColor(.5f);
        SetFreq(440.f);
        for(int i = 0; i < order; ++i)
        {
            generator_[i].Init(sample_rate_);
        }
    }

	/** Get the next sample. */
    float Process()
    {
        float gain      = 0.5f;
        float sum       = 0.0f;
        float frequency = frequency_;

        for(int i = 0; i < order; ++i)
        {
			generator_[i].SetFreq(frequency);
            sum += generator_[i].Process() * gain;
            gain *= decay_;
            frequency *= 2.0f;
        }

        return sum;
    }

	/** Set the lowest noise frequency.
		\param freq Frequency of the lowest noise source in Hz.
	*/
    void SetFreq(float freq)
    {
        frequency_ = fclamp(freq, 0.f, sample_rate_);
    }

	/** Sets the amount of high frequency noise.
		\** Works 0-1. 1 is the brightest, and 0 is the darkest.
	*/
    void SetColor(float color) { decay_ = fclamp(color, 0.f, 1.f); }

  private:
	float sample_rate_;
    float frequency_;
    float decay_;

    T generator_[order];
};
} // namespace daisysp
#endif
#endif