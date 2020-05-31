/** Simple Peak Limiter
This was extracted from pichenettes/stmlib.
Credit to pichenettes/Mutable Instruments
*/
#pragma once
#ifndef LIMITER_H
#define LIMITER_H
#include <stdlib.h>
namespace daisysp 
{
class Limiter
{
  public:
    Limiter() {}
    ~Limiter() {}
/** Initializes the Limiter instance. 
*/
    void Init();
/** Processes a block of audio through the limiter.
*in - pointer to a block of audio samples to be processed. The buffer is operated on directly.
size - size of the buffer "in"
pre_gain - amount of pre_gain applied to the signal.
*/
    void ProcessBlock(float *in, size_t size, float pre_gain);
  private:
    float peak_;
};
} // namespace daisysp
#endif
