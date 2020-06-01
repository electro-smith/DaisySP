/*  Allpass
    Allpass filter module
  
    Passes all frequencies at their original levels, with a phase shift.
  
    Original author(s) : Barry Vercoe, John ffitch
    Year : 1991
    Ported from soundpipe by Ben Sergentanis, May 2020
*/

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

    /** Init
        
        Initializes the allpass module.
        \param sample_rate  The sample rate of the audio engine being run.
    */
    void Init(float sample_rate, float* buff, size_t size);
   
    /**  Process
	 Returns next sample.
	 \param in Input sample.
	 \return Next floating point sample.
    */
    float Process(float in);
    // ~~~~

    /** SetFreq
       Sets the filter frequency (Implemented by delay time).
       \param looptime Filter looptime in seconds.
    */
    void SetFreq(float looptime);

    /** SetRevTime
        Sets the reverberation time in seconds.
        \param revtime Reverb time in seconds.
    */
    inline void SetRevTime(float revtime) { rev_time_ = revtime; }


  private:
    float  sample_rate_, rev_time_, loop_time_, prvt_, coef_, max_loop_time_;
    float* buf_;
    int    buf_pos_, mod_;
};
} // namespace daisysp
#endif
#endif
