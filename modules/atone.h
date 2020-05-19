// # ATone
//
// A first-order recursive high-pass filter with variable frequency response.
//
// ## Credits  
// * Original Author(s): Barry Vercoe, John FFitch, Gabriel Maldonado
// * Year: 1991
// * Original Location: Csound -- OOps/ugens5.c
// * Ported from soundpipe by Ben Sergentanis, May 2020

#pragma once
#ifndef DSY_ATONE_H
#define DSY_ATONE_H

#include <stdint.h>
#ifdef __cplusplus

namespace daisysp
{
	class ATone
	{
	public:
		ATone() {}
		~ATone() {}
// ### Init
//
// Initializes the ATone module.
//
// sample_rate - The sample rate of the audio engine being run. 
// 
// ~~~~
		void Init(float sample_rate);
// ~~~~

// ### Process
//
// Processes one sample through the filter and returns one sample.
//
// in - input signal 
// 
// ~~~~
		float Process(float &in);
// ~~~~

// ## Setters
// 
// ### SetFreq
// 
// Sets the cutoff frequency or half-way point of the filter.
// 
// Arguments
//
// - freq - frequency value in Hz. Range: Any positive value.
//
// ~~~~
		inline void SetFreq(float &freq) 
// ~~~~
		{
			freq_ = freq;
    		CalculateCoefficients();
		}

// ## Getters
// 
// ### GetFreq
// 
// Returns the current value for the cutoff frequency or half-way point of the filter.
//
// ~~~~
		inline float GetFreq() { return freq_; }
// ~~~~

	private:
		void CalculateCoefficients();
		float out_, prevout_, in_, freq_, c2_, sample_rate_;
	};
} // namespace daisysp
#endif
#endif
