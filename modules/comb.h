#pragma once
#ifndef DSY_COMB_H
#define DSY_COMB_H

#include <stdint.h>
#ifdef __cplusplus

#include <math.h>

namespace daisysp
{
/** Comb filter module

    Original author(s) :

    Ported from soundpipe by Ben Sergentanis, May 2020
*/
class Comb
{
  public:
    Comb() {}
    ~Comb() {}
    /** Initializes the Comb module.
        \param sample_rate - The sample rate of the audio engine being run. 
        \param buff - input buffer, kept in either main() or global space
        \param size - size of buff
    */
    void Init(float sample_rate, float* buff, size_t size);


    /** processes the comb filter
    */
    float Process(float in);


    /** Sets the frequency of the comb filter
    */
    void SetFreq(float looptime);


    /** Sets the decay time of the comb filter
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
