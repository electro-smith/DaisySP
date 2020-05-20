// # BlTriangle
//
// Band Limited Triangle Oscillator
//
// ## Credits  
// * Original Author(s): Paul Batchelor, saw2 Faust by Julius Smith
// * Ported from soundpipe by Ben Sergentanis, May 2020

#pragma once
#ifndef DSY_BLTRIANGLE_H
#define DSY_BLTRIANGLE_H

#include <stdint.h>
#ifdef __cplusplus

namespace daisysp
{
    class BlTriangle
    {
    public:
        BlTriangle() {}
	~BlTriangle() {}

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
	float fRec1_[2], fRec0_[2], fVec1_[2], fVec2_[4096],
	      freq_, amp_, Sampling_Freq_, FourOverSr_,
	      HalfSr_, QuarterSr_, SecPerSample_;
	int IOTA_;
    };
} // namespace daisysp
#endif
#endif
