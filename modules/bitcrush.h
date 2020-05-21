// #bitcrush
// bitcrush module
// ## Credits
// *Original author(s) : Paul Batchelor, 
// *Ported from soundpipe by Ben Sergentanis, May 2020

#pragma once
#ifndef DSY_BITCRUSH_H
#define DSY_BITCRUSH_H

#include <stdint.h>
#ifdef __cplusplus

namespace daisysp
{

class Bitcrush
{
    public:
    Bitcrush() {}
    ~Bitcrush() {}
  
    // ### Init
    //
    // Initializes the bitcrush module.
    //
    // sample_rate - The sample rate of the audio engine being run. 
    // 
    // ~~~~
        void Init(float sample_rate);
    // ~~~~

    // ### Process
    //
    // 
    // ~~~~
        float Process(float in);
    // ~~~~
  
    private:
        float sample_rate_;
	int crush_rate_, bit_depth_;
    };
} // namespace daisysp
#endif
#endif
