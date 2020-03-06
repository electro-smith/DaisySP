// # Line
// creates a Line segment signal

#pragma once
#ifndef LINE_H
#define LINE_H
#include <stdint.h>
#ifdef __cplusplus

namespace daisysp
{
	class Line
	{
	public:
		Line() {}
		~Line() {}
// ### Init
// Initializes Line module.
// ~~~~
		void Init(float sample_rate);
// ~~~~

// ### Process
// Processes Line segment. Returns one sample.
//
// value of finished will be updated to a 1, upon completion of the Line's trajectory.
// ~~~~
		float Process(uint8_t *finished);
// ~~~~

// ### Start
// Begin creation of Line. 
//
// Arguments:
//
// - start - beginning value
// - end - ending value
// - dur - duration in seconds of Line segment

// ~~~~
		void Start(float start, float end, float dur);
// ~~~~

	private:
		float start_, end_, dur_;
		float inc_, val_, sample_rate_;
		uint8_t finished_;
	};
} // namespace daisysp
#endif
#endif
