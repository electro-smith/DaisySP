// # Phasor
// Generates a normalized signal moving from 0-1 at the specified frequency.
//
// TODO:
//
// I'd like to make the following things easily configurable:
//
// - Selecting which channels should be initialized/included in the sequence conversion.
// - Setup a similar start function for an external mux, but that seems outside the scope of this file.

#pragma once
#ifndef DSY_PHASOR_H
#define DSY_PHASOR_H
#ifdef __cplusplus

namespace daisysp
{
	class Phasor
	{
	public:
		Phasor() {}
		~Phasor() {}

// ### Init
// 
// Initializes the Phasor module
// 
// sample rate, and freq are in Hz
// 
// initial phase is in radians
// 
// Additional Init functions have defaults when arg is not specified:
//
// - phs = 0.0f
// - freq = 1.0f

// ~~~~
		inline void Init(float sample_rate, float freq, float initial_phase) 
// ~~~~
		{
			sample_rate_ = sample_rate;
			phs_ = initial_phase;
			SetFreq(freq);
		}

// ~~~~
		inline void Init(float sample_rate, float freq) 
// ~~~~
		{
            Init(sample_rate, freq, 0.0f);
		}

// ~~~~
		inline void Init(float sample_rate) 
// ~~~~
		{ 
            Init(sample_rate, 1.0f, 0.0f);
		}


// ### Process
// processes Phasor and returns current value
//
// ~~~~
		float Process();
// ~~~~

// ## Setters

// ### SetFreq
// Sets frequency of the Phasor in Hz
// ~~~~
		void SetFreq(float freq);
// ~~~~

// ## Getters

// ### GetFreq
// Returns current frequency value in Hz
// ~~~~
		inline float GetFreq() { return freq_; }
// ~~~~

	private:
		float freq_;
		float sample_rate_, inc_, phs_;

	};
} // namespace daisysp
#endif
#endif
