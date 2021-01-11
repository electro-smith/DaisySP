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
       Random impulse train processed by a resonant filter.
       Ported by Ben Sergentanis, Jan 2021 
       @author Emilie Gillet
       @date 2016
*/
class Particle
{
  public:
    Particle() {}
    ~Particle() {}

    inline void Init(float sample_rate);

    inline float Process();

    float ProcessAux();

    void SetFreq(float frequency);

    void SetResonance(float resonance);

    void SetRandomFreq(float freq);

    void SetDensity(float density);

    void SetGain(float gain);

    void SetSpread(float spread);

    void Sync();

  private:
    float sample_rate_;
    float aux_, frequency_, density_, gain_, freq_, spread_, resonance_;
    bool  sync_;

    //from stmlib random.h
    inline float GetFloat();
    int32_t      rng_state_ = 0x8D5A61A4; //default seed

    float rand_phase_;
    float rand_freq_;

    const float ratiofrac_ = 1.f / 12.f;

    float pre_gain_;
    Svf   filter_;
};
} // namespace daisysp
#endif
#endif