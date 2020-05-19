// # BlSaw
//
// Band Limited Saw Oscillator
//
// ## Credits  
// * Original Author(s): Paul Batchelor, saw2 Faust by Julius Smith
// * Ported from soundpipe by Ben Sergentanis, May 2020

#pragma once
#ifndef DSY_BLSAW_H
#define DSY_BLSAW_H

#include <stdint.h>
#ifdef __cplusplus

namespace daisysp
{
    class BlSaw
    {
    public:
        BlSaw() {}
	~BlSaw() {}

	inline void SetFreq(float freq) { freq_ = freq; };

	inline void SetAmp(float amp)   { amp_ = amp;};
	
	void Init(float sample_rate);

	float Process();
	
    private:
	float fRec0_[2], fVec0_[2], fVec1_[2],
	      freq_, amp_, Sampling_Freq_, Nyquist_;
    };
} // namespace daisysp
#endif
#endif
