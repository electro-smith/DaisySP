// #Comb
// Comb filter module
// ## Credits
// *Original author(s) :
// *Ported from soundpipe by Ben Sergentanis, May 2020

#pragma once
#ifndef DSY_COMB_H
#define DSY_COMB_H

#include <stdint.h>
#ifdef __cplusplus

#include <math.h>
#include "dsp.h"

namespace daisysp
{

template <typename T, size_t size>
class Comb
{
    public:
    Comb() {}
    ~Comb() {}
  
    // ### Init
    //
    // Initializes the Comb module.
    //
    // sample_rate - The sample rate of the audio engine being run. 
    // 
    // ~~~~
    void Init (float sample_rate, Buffer <T,size> buf);
    // ~~~~

    // ### Process
    //
    // 
    // ~~~~
        float Process(float in);
    // ~~~~

    // #Setters
    //
    // ### SetFreq
    //
    // 
    // ~~~~
	inline void SetFreq(float looptime);
    // ~~~~

    // #Setters
    //
    // ### SetRevTime
    //
    // ~~~~
        inline void SetRevTime(float revtime) { rev_time_ = revtime; }
    // ~~~~
	
    private:
	float sample_rate_, rev_time_, loop_time_, prvt_, coef_, max_loop_time_;
	Buffer <T, size> buf_;
	int  buf_pos_, mod_;
    };
} // namespace daisysp

#endif
#endif
