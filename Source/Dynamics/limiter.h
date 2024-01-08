/*
Copyright (c) 2020 Electrosmith, Corp, Emilie Gillet

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/

#pragma once
#ifndef LIMITER_H
#define LIMITER_H
#include <stdlib.h>
namespace daisysp
{
/** Simple Peak Limiter

This was extracted from pichenettes/stmlib.

Credit to pichenettes/Mutable Instruments
*/
class Limiter
{
  public:
    Limiter() {}
    ~Limiter() {}
    /** Initializes the Limiter instance. 
    */
    void Init();

    /** Processes a block of audio through the limiter.
        \param in - pointer to a block of audio samples to be processed. The buffer is operated on directly.
        \param size - size of the buffer "in"
        \param pre_gain - amount of pre_gain applied to the signal.
    */
    void ProcessBlock(float *in, size_t size, float pre_gain);

  private:
    float peak_;
};
} // namespace daisysp
#endif
