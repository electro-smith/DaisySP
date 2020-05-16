// # Metro 
// 
// Creates a clock signal at a specific frequency.
// 

#pragma once
#ifndef DSY_METRO_H
#define DSY_METRO_H
#include <stdint.h>
#ifdef __cplusplus

namespace daisysp
{
	class Metro
	{
	public:
		Metro() {}
		~Metro() {}

// ### Init
// 
// Initializes Metro module.
// 
// Arguments:
// - freq: frequency at which new clock signals will be generated
// 	Input Range: 
// - sample_rate: sample rate of audio engine
// 	Input range: 

// ~~~~
		void Init(float freq, float sample_rate);
// ~~~~

// ### Process
// checks current state of Metro object and updates state if necesary.

// ~~~~
		uint8_t Process();
// ~~~~

// ### Reset
// resets phase to 0
// ~~~~
		inline void Reset() { phs_ = 0.0f; }
// ~~~~

// ## Setters
// ### SetFreq
// Sets frequency at which Metro module will run at.

// ~~~~
		void SetFreq(float freq);
// ~~~~

// ## Getters
// ### GetFreq
// Returns current value for frequency.

// ~~~~
		inline float GetFreq() { return freq_; }
// ~~~~

	private:
		float freq_;
		float phs_, sample_rate_, phs_inc_;

	};
} // namespace daisysp
#endif
#endif
