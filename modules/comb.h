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

namespace daisysp
{

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
        inline void SetLoopTime(float looptime) { loop_time_ = fminf (0.5f, looptime); buf_size_ = (int) (loop_time_ * sample_rate_); }
    // ~~~~

    // #Setters
    //
    // ### SetRevTime
    //
    // ~~~~
        inline void SetRevTime(float revtime) { rev_time_ = revtime; }
    // ~~~~
	
    private:
	float sample_rate_, rev_time_, loop_time_, prvt_, coef_,
	      buf_[96000];
	int   buf_size_, buf_pos_;
    };
} // namespace daisysp
#endif
#endif
