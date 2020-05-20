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

    // #Setters	
    //
    // ###SetFreq
    //
    // - Float freq: Set oscillator frequency in Hz.
    //
    // ~~~~
        inline void SetFreq(float freq) { freq_ = freq; };
    // ~~~~

    // ###SetAmp
    //
    // - Float freq: Set oscillator amplitude, 0 to 1.
    //
    // ~~~~
	inline void SetAmp(float amp)   { amp_ = amp;};
    // ~~~~

    // ###Init
    //
    // -Initialize oscillator. Defaults to 440Hz, .5 amplitude.
    //
    // #Argument:  float sample_rate: audio engine sample rate.
    //
    // ~~~~
	void Init(float sample_rate);
    // ~~~~

    // ###Process
    //
    // - Get next floating point oscillator sample.
    //
    // ~~~~
	float Process();
    // ~~~~

    private:
	float fRec0_[2], fVec0_[2], fVec1_[2],
	      freq_, amp_, Sampling_Freq_, TwoOverSr_;
    };
} // namespace daisysp
#endif
#endif
