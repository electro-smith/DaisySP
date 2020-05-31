#pragma once
#ifndef DSY_TONE_H
#define DSY_TONE_H

#include <stdint.h>
#ifdef __cplusplus

namespace daisysp
{
/** A first-order recursive low-pass filter with variable frequency response.
*/
class Tone
{
  public:
    Tone() {}
    ~Tone() {}
    /** Initializes the Tone module.
        sample_rate - The sample rate of the audio engine being run. 
    */
    void Init(float sample_rate);


    /** Processes one sample through the filter and returns one sample.
        in - input signal 
    */
    float Process(float &in);

    /** Sets the cutoff frequency or half-way point of the filter.

        \param freq - frequency value in Hz. Range: Any positive value.
    */
    inline void SetFreq(float &freq)
    {
        freq_ = freq;
        CalculateCoefficients();
    }

    /** 
        \return the current value for the cutoff frequency or half-way point of the filter.
    */
    inline float GetFreq() { return freq_; }

  private:
    void  CalculateCoefficients();
    float out_, prevout_, in_, freq_, c1_, c2_, sample_rate_;
};
} // namespace daisysp
#endif
#endif
