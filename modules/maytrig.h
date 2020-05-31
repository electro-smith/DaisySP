/** Probabilistic trigger  module
*Original author(s) : Paul Batchelor
*Ported from soundpipe by Ben Sergentanis, May 2020
*/

#pragma once
#ifndef DSY_MAYTRIG_H
#define DSY_MAYTRIG_H

#include <stdint.h>
#ifdef __cplusplus

namespace daisysp
{

class Maytrig
{
    public:
    Maytrig() {}
    ~Maytrig() {}

/** * Returns given a probability 0 to 1, returns true or false.
*(1 always returns true, 0 always false)
*/
        inline float Process(float prob){ return ((float)rand() / RAND_MAX) <= prob ? true : false; }
  
    private:

    };
} // namespace daisysp
#endif
#endif
