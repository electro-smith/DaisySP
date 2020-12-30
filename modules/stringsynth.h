// A mixture of 7 sawtooth and square waveforms in the style of divide-down
// organs:
//
// [0]     [1]        [2]     [3]        [4]     [5]        [6]
// Saw 8', Square 8', Saw 4', Square 4', Saw 2', Square 2', Saw 1'
//
// Internally, this renders 4 band-limited sawtooths, from 8' to 1' from a
// single phase counter. The square waveforms are obtained by algebraic
// manipulations on the sawtooths, using the identity:
// Square 16' = 2 Sawtooth 16' - Sawtooth 8'

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

    void Init(float sample_rate);
    float Process();

	void SetFreq(float freq);
	void SetAmplitudes(const float* amplitudes);
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
	bool recalc_;
	
	inline float ThisBlepSample(float t);
	inline float NextBlepSample(float t);

};
} // namespace daisysp
#endif
#endif