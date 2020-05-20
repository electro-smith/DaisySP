// # BlSquare
//
// Band Limited Square Oscillator
//
// ## Credits  
// * Original Author(s): Paul Batchelor, saw2 Faust by Julius Smith
// * Ported from soundpipe by Ben Sergentanis, May 2020

#pragma once
#ifndef DSY_BLSQUARE_H
#define DSY_BLSQUARE_H

#include <stdint.h>
#ifdef __cplusplus

namespace daisysp
{
    class BlSquare
    {
    public:
        BlSquare() {}
	~BlSquare() {}

    // #Setters	
    //
    // ###SetFreq
    //
    // - Float freq: Set oscillator frequency in Hz.
    // ~~~~
        inline void SetFreq(float freq) { freq_ = freq; };
    // ~~~~

    // ###SetAmp
    //
    // - Float amp: Set oscillator amplitude, 0 to 1.
    // ~~~~
	inline void SetAmp(float amp)   { amp_ = amp;};
    // ~~~~

    // ###SetPw
    //
    // - Float pw: Set pulsewidth, 0 to 1. (no thru 0 at the moment)
    // ~~~~
	inline void SetPw(float pw)   { pw_ = 1 - pw;};
    // ~~~~
	
    // ###Init
    //
    // -Initialize oscillator. Defaults to 440Hz, .5 amplitude.
    // #Argument:  float sample_rate: audio engine sample rate.
    // ~~~~
	void Init(float sample_rate);
    // ~~~~

    // ###Process
    //
    // - Get next floating point oscillator sample.
    // ~~~~
	float Process();
    // ~~~~

    private:
	float fRec0_[2], fVec1_[2], fVec2_[4096],
	      freq_, amp_, pw_, Sampling_Freq_, QuarterSr_,
	      SecPerSample_;
	int IOTA_;
    };
} // namespace daisysp
#endif
#endif
