/** Autowah module
*Original author(s) :
*Ported from soundpipe by Ben Sergentanis, May 2020
*/

#pragma once
#ifndef DSY_AUTOWAH_H
#define DSY_AUTOWAH_H

#include <stdint.h>
#ifdef __cplusplus

namespace daisysp
{

class Autowah
{
    public:
    Autowah() {}
    ~Autowah() {}
  

/** Initializes the Autowah module.
sample_rate - The sample rate of the audio engine being run. 
*/
        void Init(float sample_rate);


        float Process(float in);



/** * float wah : set wah amount
*/
	inline void SetWah(float wah) { wah_ = wah; }
	

/** * float drywet : set effect dry/wet
*/
	inline void SetDryWet(float drywet) { wet_dry_ = drywet; }


/** * float level : set wah level
*/
	inline void SetLevel(float level) { level_ = level; }
    private:
	float sampling_freq_, const1_, const2_, const4_,
	      wah_, level_, wet_dry_,
	      rec0_[3], rec1_[2], rec2_[2], rec3_[2], rec4_[2], rec5_[2];
	
    };
} // namespace daisysp
#endif
#endif
