#pragma once
#ifndef DSY_AUTOWAH_H
#define DSY_AUTOWAH_H

#include <stdint.h>
#ifdef __cplusplus

namespace daisysp
{
/** Autowah module

    Original author(s) :

    Ported from soundpipe by Ben Sergentanis, May 2020
*/
class Autowah
{
  public:
    Autowah() {}
    ~Autowah() {}
    /** Initializes the Autowah module.
        \param sample_rate - The sample rate of the audio engine being run. 
    */
    void Init(float sample_rate);


    /** Initializes the Autowah module.
        \param in - input signal to be wah'd
    */
    float Process(float in);


    /** sets wah
        \param wah : set wah amount, , 0...1.0
    */
    inline void SetWah(float wah) { wah_ = wah; }
    /** sets mix amount
        \param drywet : set effect dry/wet, 0...100.0
    */
    inline void SetDryWet(float drywet) { wet_dry_ = drywet; }
    /** sets wah level
        \param level : set wah level, 0...1.0
    */
    inline void SetLevel(float level) { level_ = level; }

  private:
    float sampling_freq_, const1_, const2_, const4_, wah_, level_, wet_dry_,
        rec0_[3], rec1_[2], rec2_[2], rec3_[2], rec4_[2], rec5_[2];
};
} // namespace daisysp
#endif
#endif
