#pragma once
#ifndef DSY_BITCRUSH_H
#define DSY_BITCRUSH_H

#include <stdint.h>
#ifdef __cplusplus

namespace daisysp
{

/** bitcrush module
*Original author(s) : Paul Batchelor, 
*Ported from soundpipe by Ben Sergentanis, May 2020
*/
class Bitcrush
{
    public:
    Bitcrush() {}
    ~Bitcrush() {}
  

/** Initializes the bitcrush module.
sample_rate - The sample rate of the audio engine being run. 
*/
        void Init(float sample_rate);


/** 
*/
        float Process(float in);



/** * int bitdepth : Sets bit depth.	
*/
        inline void SetBitDepth(int bitdepth) { bit_depth_ = bitdepth; }


/** * float crushrate : Sets rate to downsample to.	
*/
        inline void SetCrushRate(float crushrate) { crush_rate_ = crushrate; }
	
    private:
        float sample_rate_, crush_rate_;
	int  bit_depth_;
    };
} // namespace daisysp
#endif
#endif
