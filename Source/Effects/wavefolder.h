/*
Copyright (c) 2020 Electrosmith, Corp, Nick Donaldson

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/

#pragma once
#ifndef DSY_WAVEFOFOLDER_H
#define DSY_WAVEFOFOLDER_H

#include <stdint.h>
#include "Utility/dcblock.h"
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
        \param gain Set input gain.
        Supports negative values for thru-zero
    */
    inline void SetGain(float gain) { gain_ = gain; }
    /** 
        \param offset Offset odded to input (pre-gain) for asymmetrical folding.
    */
    inline void SetOffset(float offset) { offset_ = offset; }

  private:
    float gain_, offset_;
};
} // namespace daisysp
#endif
#endif
