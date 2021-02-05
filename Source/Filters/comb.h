#pragma once
#ifndef DSY_COMB_H
#define DSY_COMB_H
#ifdef __cplusplus

#include "Utility/dsp.h"

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


    /** Sets the period of the comb filter in seconds
    */
    void SetPeriod(float looptime);

    /** Sets the frequency of the comb filter in Hz
    */
    inline void SetFreq(float freq)
    {
        if(freq > 0)
        {
            SetPeriod(1.f / freq);
        }
    }

    /** Sets the decay time of the comb filter
    */
    inline void SetRevTime(float revtime) { rev_time_ = revtime; }

  private:
    float  sample_rate_, rev_time_, loop_time_, prvt_, coef_, max_loop_time_;
    float* buf_;
    size_t buf_pos_, mod_, max_size_;
};
} // namespace daisysp

#endif
#endif
