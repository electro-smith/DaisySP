// # CrossFade
// 
// Performs a CrossFade between two signals
// 
// Original author: Paul Batchelor
//
// Ported from Soundpipe by Andrew Ikenberry
// added curve option for constant power, etc.
// 
// TODO:
// 
// - implement exponential curve Process
// - implement logarithmic curve Process

#pragma once
#ifndef DSY_CROSSFADE_H
#define DSY_CROSSFADE_H
#include <stdint.h>
#ifdef __cplusplus

namespace daisysp
{

// ## Curve Options
// Curve applied to the CrossFade
// 
// - LIN = linear
// - CPOW = constant power
// - LOG = logarithmic
// - EXP  exponential
// - LAST = end of enum (used for array indexing)
// ~~~~
	enum 
	{
		CROSSFADE_LIN,
		CROSSFADE_CPOW,
		CROSSFADE_LOG,
		CROSSFADE_EXP,
		CROSSFADE_LAST,
	};
// ~~~~

	class CrossFade
	{
	public:
		CrossFade() {}
		~CrossFade() {}

        // ### init
        // Initializes CrossFade module
        //
        // Defaults
        //
        // - current position = .5
        // - curve = linear
        //
        // ~~~~
        inline void init(int curve)
// ~~~~
        {
            pos_ = 0.5f;
            curve_ = curve < CROSSFADE_LAST ? curve : CROSSFADE_LIN;
        }
        // ~~~~
		inline void init() 
// ~~~~
		{
            init(CROSSFADE_LIN);
        }
        // ### Process
        // processes CrossFade and returns single sample
        //
        // ~~~~
        float Process(float &in1, float &in2);
// ~~~~

// ## Setters

// ### SetPos
// Sets position of CrossFade between two input signals
// 
// Input range: 0 to 1
// ~~~~
		inline void SetPos(float pos) { pos_ = pos; }
// ~~~~

// ### SetCurve
// 
// Sets current curve applied to CrossFade 
// 
// Expected input: See [Curve Options](##curve-options)
// ~~~~
		inline void SetCurve(uint8_t curve) { curve_ = curve; }
// ~~~~

// ## Getters

// ### GetPos
// Returns current position
// ~~~~
		inline float GetPos(float pos) { return pos_; }
// ~~~~

// ### GetCurve
// Returns current curve
// ~~~~
		inline uint8_t GetCurve(uint8_t curve) { return curve_; }
// ~~~~

	private:
		float pos_;
		uint8_t curve_;
	};
} // namespace daisysp
#endif
#endif
