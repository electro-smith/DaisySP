#pragma once
#ifndef DSY_NLFILT_H
#define DSY_NLFILT_H
#include <stdlib.h>
#include <stdint.h>
#define DSY_NLFILT_MAX_DELAY 1024

namespace daisysp
{
/** Non-linear filter
    
    port by: Stephen Hensley, December 2019
    
    The four 5-coefficients: a, b, d, C, and L are used to configure different filter types.

    Structure for Dobson/Fitch nonlinear filter 

    Revised Formula from Risto Holopainen 12 Mar 2004

    `Y{n} =tanh(a Y{n-1} + b Y{n-2} + d Y^2{n-L} + X{n} - C)`

    Though traditional filter types can be made, 
    the effect will always respond differently to different input.

    This Source is a heavily modified version of the original
    source from Csound.

    \todo make this work on a single sample instead of just on blocks at a time.
    */
class NlFilt
{
  public:
    /** Initializes the NlFilt object.
        */
    void Init();

    /** Process the array pointed to by \*in and updates the output to \*out;
        This works on a block of audio at once, the size of which is set with the size. 
        */
    void ProcessBlock(float *in, float *out, size_t size);


    /** inputs these are the five coefficients for the filter.
        */
    inline void SetCoefficients(float a, float b, float d, float C, float L)
    {
        a_ = a;
        b_ = b;
        d_ = d;
        C_ = C;
        L_ = L;
    }


    /** Set Coefficient a
        */
    inline void SetA(float a) { a_ = a; }
    /** Set Coefficient b
        */
    inline void SetB(float b) { b_ = b; }
    /** Set Coefficient d
        */
    inline void SetD(float d) { d_ = d; }
    /** Set Coefficient C
        */
    inline void SetC(float C) { C_ = C; }
    /** Set Coefficient L
        */
    inline void SetL(float L) { L_ = L; }

  private:
    int32_t Set();

    float   in_, a_, b_, d_, C_, L_;
    float   delay_[DSY_NLFILT_MAX_DELAY];
    int32_t point_;
};
} // namespace daisysp

#endif
