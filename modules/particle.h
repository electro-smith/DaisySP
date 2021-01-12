#pragma once
#ifndef DSY_PARTICLE_H
#define DSY_PARTICLE_H

#include "Svf.h"
#include <stdint.h>
#ifdef __cplusplus

/** @file particle.h */

namespace daisysp
{
/**  
       @brief Random impulse train processed by a resonant filter.
	   @author Ported by Ben Sergentanis 
	   @date Jan 2021 
	   Noise processed by a sample and hold running at a target frequency. \n \n
	   Ported from pichenettes/eurorack/plaits/dsp/noise/particle.h \n
	   to an independent module. \n
	   Original code written by Emilie Gillet in 2016. \n
*/
class Particle
{
  public:
    Particle() {}
    ~Particle() {}

    void Init(float sample_rate);

    float Process();

    float GetNoise();

    void SetFreq(float frequency);

    void SetResonance(float resonance);

    void SetRandomFreq(float freq);

    void SetDensity(float density);

    void SetGain(float gain);

    void SetSpread(float spread);

    void SetSync(bool sync);

  private:
    float sample_rate_;
    float aux_, frequency_, density_, gain_, spread_, resonance_;
    bool  sync_;

    //from stmlib random.h
    inline float GetFloat();
    int32_t      rng_state_ = 0x8D5A61A4; //default seed
    float rand_frac_ = 1.f / RAND_MAX;

    float rand_phase_;
    float rand_freq_;

    const float ratiofrac_ = 1.f / 12.f;

    float pre_gain_;
    Svf   filter_;
};
} // namespace daisysp
#endif
#endif