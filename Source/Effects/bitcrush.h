#pragma once
#ifndef DSY_BITCRUSH_H
#define DSY_BITCRUSH_H

#include <stdint.h>
#ifdef __cplusplus

namespace daisysp
{
/** bitcrush module

Original author(s) : Paul Batchelor, 

Ported from soundpipe by Ben Sergentanis, May 2020
*/
class Bitcrush
{
  public:
    Bitcrush() {}
    ~Bitcrush() {}
    /** Initializes the bitcrush module.
        \param sample_rate - The sample rate of the audio engine being run. 
    */
    void Init(float sample_rate);


    /** bit crushes and downsamples the input
    */
    float Process(float in);


    /** adjusts bitdepth
        \param bitdepth : Sets bit depth, 0...16
    */
    inline void SetBitDepth(int bitdepth) { bit_depth_ = bitdepth; }
    /** adjusts the downsampling frequency
        \param crushrate : Sets rate to downsample to, 0...SampleRate
    */
    inline void SetCrushRate(float crushrate) { crush_rate_ = crushrate; }

  private:
    float sample_rate_, crush_rate_;
    int   bit_depth_;
};
} // namespace daisysp
#endif
#endif
