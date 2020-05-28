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
    void Init(float sample_rate, float* buff, size_t size);
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
	void SetFreq(float looptime);
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
	  prvt_, coef_, max_loop_time_;
	float* buf_;
	int buf_pos_, mod_;
    };
} // namespace daisysp
#endif
#endif
