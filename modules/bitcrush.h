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

    // #Setters
    //
    // ###SetBitdepth
    //
    // * int bitdepth : Sets bit depth.	
    //
    // ~~~~
        inline void SetBitDepth(int bitdepth) { bit_depth_ = bitdepth; }
    // ~~~~

    // ###SetCrushRate
    //
    // * float crushrate : Sets rate to downsample to.	
    //
    // ~~~~
        inline void SetCrushRate(float crushrate) { crush_rate_ = crushrate; }
    // ~~~~
	
    private:
        float sample_rate_, crush_rate_;
	int  bit_depth_;
    };
} // namespace daisysp
#endif
#endif
