#pragma once
#ifndef DSY_DRIP_H
#define DSY_DRIP_H

#include <stdint.h>
#ifdef __cplusplus

/**  @file drip.h */

namespace daisysp
{
/** 
    Imitates the sound of dripping water via Physical Modeling Synthesis. \n 
    Ported from soundpipe by Ben Sergentanis, May 2020 
    @author Perry Cook  
    @date 2000  
*/
class Drip
{
  public:
    Drip() {}
    ~Drip() {}

    /** 
        Initializes the Drip module. 
        \param sample_rate The sample rate of the audio engine being run.
    \param dettack The period of time over which all sound is stopped.
    */
    void Init(float sample_rate, float dettack);

    /** 
        Process the next floating point sample.
        \param trig If true, begins a new drip.
        \return Next sample.
    */
    float Process(bool trig);

  private:
    float gains0_, gains1_, gains2_, kloop_, dettack_, num_tubes_, damp_,
        shake_max_, freq_, freq1_, freq2_, amp_, snd_level_, outputs00_,
        outputs01_, outputs10_, outputs11_, outputs20_, outputs21_,
        total_energy_, center_freqs0_, center_freqs1_, center_freqs2_,
        num_objects_save_, sound_decay_, system_decay_, finalZ0_, finalZ1_,
        finalZ2_, coeffs01_, coeffs00_, coeffs11_, coeffs10_, coeffs21_,
        coeffs20_, shake_energy_, shake_damp_, shake_max_save_, num_objects_,
        sample_rate_, res_freq0_, res_freq1_, res_freq2_, inputs1_, inputs2_;

    int   my_random(int max);
    float noise_tick();
};
} // namespace daisysp
#endif
#endif
