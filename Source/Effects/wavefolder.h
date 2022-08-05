#pragma once
#ifndef DSY_WAVEFOFOLDER_H
#define DSY_WAVEFOFOLDER_H

#include <stdint.h>
#ifdef __cplusplus

namespace daisysp
{
/** Basic wavefolder module.

Amplitude of input determines level of folding.
Amplitudes of magnitude > 1.0 will start to fold.

Original author(s) : Nick Donaldson
Year : 2022
*/
class Wavefolder
{
  public:
    Wavefolder() {}
    ~Wavefolder() {}
    /** Initializes the wavefolder module.
    */
    void Init();
    /** applies wavefolding to input 
    */
    float Process(float in);
    /** 
        \param gain : set input gain, 
        supports negative values for thru-zero
    */
    inline void SetGain(float gain) { gain_ = gain; }

  private:
    float gain_;
};
} // namespace daisysp
#endif
#endif
