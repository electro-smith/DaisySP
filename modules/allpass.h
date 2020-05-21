// #Allpass
// Allpass module
// ## Credits
// *Original author(s) : Barry Vercoe, John ffitch
// *Year : 1991
// *Ported from soundpipe by Ben Sergentanis, May 2020

#pragma once
#ifndef DSY_ALLPASS_H
#define DSY_ALLPASS_H

#include <stdint.h>
#ifdef __cplusplus

namespace daisysp
{

class Allpass
{
    public:
    Allpass() {}
    ~Allpass() {}
  
    // ### Init
    //
    // Initializes the xyz module.
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
	float coef_, sample_rate_, rev_time_, loop_time_, buf_size_,
	  prvt_, coef_;
	int buff_pos;
    };
} // namespace daisysp
#endif
#endif
