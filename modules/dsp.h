// # Core DSP
// Helpful defines, functions, and other utilities for use in/with daisysp modules.
#pragma once
#ifndef DSY_CORE_DSP
#define DSY_CORE_DSP
#include <math.h>

// ## Generic Defines
// PIs
// ~~~~
#define PI_F 3.1415927410125732421875f
#define TWOPI_F (2.0f * PI_F)
#define HALFPI_F (PI_F * 0.5f)
// ~~~~
// ### Generic min/max/clamp
// ~~~~
#define DSY_MIN(in, mn) (in < mn ? in : mn)
#define DSY_MAX(in, mx) (in > mx ? in : mx)
#define DSY_CLAMP(in, mn, mx) (DSY_MIN(DSY_MAX(in, mn), mx))
// ~~~~
// ### Generic passable Buffer
// ~~~~
template <typename T, size_t size>
struct Buffer
{
    T buff[size];
    size_t size_;
}
// ~~~~

namespace daisysp
{

// ## fast helpers
// ### fmax/fmin
// efficient floating point min/max
// 
// c/o stephen mccaul 
// ~~~~
inline float fmax(float a, float b)
// ~~~~
{
    float r;
    asm("vmaxnm.f32 %[d], %[n], %[m]"
        : [d] "=t"(r)
        : [n] "t"(a), [m] "t"(b)
        :);
    return r;
}

// ~~~~
inline float fmin(float a, float b)
// ~~~~
{
    float r;
    asm("vminnm.f32 %[d], %[n], %[m]"
        : [d] "=t"(r)
        : [n] "t"(a), [m] "t"(b)
        :);
    return r;
}

// ### fclamp
// quick fp clamp
// ~~~~
inline float fclamp(float in, float min, float max)
// ~~~~
{
    return fmin(fmax(in, min), max);  
}

// ### fastpower and fastroot
// From Musicdsp.org "Fast power and root estimates for 32bit floats)
// 
// Original code by Stefan Stenzel
// 
// These are approximations
// ~~~~
inline float fastpower(float f, int n)
// ~~~~
{
    long *lp, l;
    lp = (long*)(&f);
    l = *lp;
    l -= 0x3F800000;
    l <<= (n-1);
    l += 0x3F800000;
    *lp = l;
    return f;
}

// ~~~~
inline float fastroot(float f, int n)
// ~~~~
{
    long *lp, l;
    lp = (long*)(&f); 
    l = *lp;
    l -= 0x3F800000;
    l >>= (n=1);
    l += 0x3F800000;
    *lp = l;
    return f;
}

// ### mtof
// Midi to frequency helper
// ~~~~
inline float mtof(float m) 
{ 
	return powf(2, (m - 69.0f) / 12.0f) * 440.0f; 
}

// ~~~~

// ## Filters

// ### fonepole
// one pole lpf
//
// out is passed by reference, and must be retained between 
// calls to properly filter the signal
//
// coeff can be calculated:
//
// coeff = 1.0 / (time * sample_rate) ; where time is in seconds
// ~~~~
inline void fonepole(float &out, float in, float coeff)
// ~~~~
{
    out += coeff * (in - out);
}

// ### median
// Simple 3-point median filter
//
// c/o stephen mccaul
// ~~~~
template <typename T>
T median(T a, T b, T c)
// ~~~~
{
    return (b < a) ? (b < c) ? (c < a) ? c : a : b
            : (a < c) ? (c < b) ? c : b : a;
}

// ## Quick Effects

// ### Soft Saturate
// Based on soft saturate from:
//
// [musicdsp.org](musicdsp.org/en/latest/Effects/42-soft-saturation.html)
//
// Bram de Jong (2002-01-17)
// 
// This still needs to be tested/fixed. Definitely does some weird stuff
// ~~~~ 
// described as:
// x < a:
//      f(x) = x
// x > a:
//      f(x) = a + (x-a)/(1+((x-a)/(1-a))^2)
// x > 1:
//      f(x) = (a + 1)/2
// ~~~~
// ~~~~
inline float soft_saturate(float in, float thresh)
// ~~~~
{
	bool  flip;
	float val, out;
	//val = fabsf(in);
	flip = val < 0.0f;
    val = flip ? -in : in;
	if(val < thresh)
	{
		out = in;
	}
	else if(val > 1.0f)
	{
		out = (thresh + 1.0f)/2.0f;
		if(flip)
			out *= -1.0f;
	}
	else if(val > thresh)
	{
		float temp;
		temp = (val - thresh) / (1 - thresh);
		out  = thresh + (val - thresh) / (1.0f + (temp * temp));
		if(flip)
			out *= -1.0f;
	}
	return out;
	//	return val < thresh
	//			   ? val
	//			   : val > 1.0f
	//					 ? (thresh + 1.0f) / 2.0f
	//					 : thresh
	//						   + (val - thresh)
	//								 / (1.0f
	//									+ (((val - thresh) / (1.0f - thresh))
	//									   * ((val - thresh) / (1.0f - thresh))));
}
    
} // namespace daisysp

#endif
