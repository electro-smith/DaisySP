#pragma once
#ifndef DSY_PARTICLE_H
#define DSY_PARTICLE_H

#include "Filters/svf.h"
#include <stdint.h>
#include <cstdlib>
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

    /** Initialize the module
        \param sample_rate Audio engine sample rate.
    */
    void Init(float sample_rate);

    /** Get the next sample */
    float Process();

    /** Get the raw noise output. Must call Process() first. */
    float GetNoise();

    /** Set the resonant filter frequency
        \param freq Frequency in Hz
    */
    void SetFreq(float frequency);

    /** Set the filter resonance
        \param resonance Works 0-1
    */
    void SetResonance(float resonance);

    /** How often to randomize filter frequency
        \param freq Frequency in Hz.
    */
    void SetRandomFreq(float freq);

    /** Noise density
        \param Works 0-1.
    */
    void SetDensity(float density);

    /** Overall module gain
        \param Works 0-1.
    */
    void SetGain(float gain);

    /** How much to randomize the set filter frequency.
        \param spread Works over positive numbers.
    */
    void SetSpread(float spread);

    /** Force randomize the frequency.
        \param sync True to randomize freq.
    */
    void SetSync(bool sync);

  private:
    static constexpr float kRandFrac  = 1.f / (float)RAND_MAX;
    static constexpr float kRatioFrac = 1.f / 12.f;
    float                  sample_rate_;
    float aux_, frequency_, density_, gain_, spread_, resonance_;
    bool  sync_;


    float rand_phase_;
    float rand_freq_;


    float pre_gain_;
    Svf   filter_;
};
} // namespace daisysp
#endif
#endif
