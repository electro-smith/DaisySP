// #Allpass
// Allpass filter module
// ## Credits
// *Original author(s) : Barry Vercoe, John ffitch
// *Year : 1991
// *Ported from soundpipe by Ben Sergentanis, May 2020

#pragma once
#ifndef DSY_ALLPASS_H
#define DSY_ALLPASS_H

#include <stdint.h>
#ifdef __cplusplus

#include <math.h>

namespace daisysp
{

class Allpass
{
    public:
    Allpass() {}
    ~Allpass() {}
  
    // ### Init
    //
    // Initializes the allpass module.
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
    // ### SetLoopTime
    //
    // 
    // ~~~~
        inline void SetLoopTime(float looptime) { loop_time_ = fminf (0.5f, looptime); }
    // ~~~~

    // #Setters
    //
    // ### SetRevTime
    //
    // ~~~~
        inline void SetRevTime(float revtime) { rev_time_ = revtime; }
    // ~~~~
	
    
    private:
	float sample_rate_, rev_time_, loop_time_, 
	  prvt_, coef_, buf_[96000];
	int buf_pos_, buf_size_;
    };
} // namespace daisysp
#endif
#endif
