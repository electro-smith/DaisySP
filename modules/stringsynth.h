#pragma once
#ifndef DSY_STRINGSYNTH_H
#define DSY_STRINGSYNTH_H

#include <stdint.h>
#ifdef __cplusplus

/** @file stringsynth.h */

namespace daisysp
{
/**  
       String Synth Oscillator Module \n 
	   A mixture of 7 sawtooth and square waveforms in the style of divide-down organs \n
       Ported by Ben Sergentanis, Dec 2020 
       @author Emilie Gillet
       @date 2016
*/

class StringSynthOscillator
{
  public:
    StringSynthOscillator() {}
    ~StringSynthOscillator() {}

    /** Init string synth module
		\param sample_rate Audio engine sample rate
	*/
    void Init(float sample_rate);

    /** Get next floating point sample 
	*/
    float Process();

    /** Set oscillator frequency (8' oscillator)
		\param freq Frequency in Hz
	*/
    void SetFreq(float freq);

    /** Set amplitudes of 7 oscillators. 0-6 are Saw 8', Square 8', Saw 4', Square 4', Saw 2', Square 2', Saw 1'
		\param amplitudes array of 7 floating point amplitudes. Must sum to 1.
	*/
    void SetAmplitudes(const float* amplitudes);

    /** Set a single amplitude
		\param amp Amplitude to set.
		\param idx Which wave's amp to set
	*/
    void SetSingleAmp(float amp, int idx);

    /** Set overall gain.
		\param gain Gain to set. 0-1.
	*/
    void SetGain(float gain);

  private:
    // Oscillator state.
    float phase_;
    float next_sample_;
    int   segment_;
    float gain_;
    float registration_[7];
    float unshifted_registration_[7];

    float frequency_;
    float saw_8_gain_;
    float saw_4_gain_;
    float saw_2_gain_;
    float saw_1_gain_;

    float sample_rate_;
    bool  recalc_, recalc_gain_;

    inline float ThisBlepSample(float t);
    inline float NextBlepSample(float t);
    bool         cmp(float a, float b) { return fabsf(a - b) > .0000001; }
};
} // namespace daisysp
#endif
#endif