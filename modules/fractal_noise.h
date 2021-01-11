#pragma once
#ifndef DSY_FORMANTOSCILLATOR_H
#define DSY_FORMANTOSCILLATOR_H

#include <stdint.h>
#ifdef __cplusplus

/** @file fractal_noise.h */

namespace daisysp
{
/**  
       Stack octaves of a noise source to generate a fractal noise. \n  
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

    void Init()
    {
        SetDecay(.5f);
        SetFreq(440.f);
        for(int i = 0; i < order; ++i)
        {
            generator_[i].Init();
        }
    }

    float Process()
    {
        float gain      = 0.5f;
        float sum       = 0.0f;
        float frequency = frequency_;

        for(int i = 0; i < order; ++i)
        {
            sum += generator_[i].Process(frequency) * gain;
            gain *= decay_;
            frequency *= 2.0f;
        }

        return sum;
    }

    void SetFreq(float freq)
    {
        freq       = freq / sample_rate_;
        frequency_ = fclamp(freq, 0.f, 1.f);
    }

    void SetDecay(float decay) { decay_ = fclamp(decay, 0.f, 1.f); }

  private:
    float frequency_;
    float decay_;

    T generator_[order];
};
} // namespace daisysp
#endif
#endif