// #bal
// Balances two sound sources. Sig is boosted to the level of comp.
//
// ## Credits
// *Original author(s) : Barry Vercoe, john ffitch, Gabriel Maldonado
// *Year: 1991
// *Ported from soundpipe by Ben Sergentanis, May 2020

#pragma once
#ifndef DSY_BAL_H
#define DSY_BAL_H

#include <stdint.h>
#ifdef __cplusplus

namespace daisysp
{

class Bal
{
    public:
    Bal() {}
    ~Bal() {}
  
    // ### Init
    //
    // Initializes the bal module.
    //
    // sample_rate - The sample rate of the audio engine being run. 
    // 
    // ~~~~
        void Init(float sample_rate);
    // ~~~~

    // ### Process
    //
    // 
    // ~~~~
        float Process(float sig, float comp);
    // ~~~~

    // # Setters
    //
    // ### SetCutoff
    //
    // * float cutoff : Sets half power point of special internal cutoff filter.
    //defaults to 10
    // ~~~~
	inline void SetCutoff (float cutoff) { ihp_ = cutoff; }
    // ~~~~
	
    private:
	float sample_rate_, ihp_, c2_, c1_, prvq_, prvr_, prva_;
    };
} // namespace daisysp
#endif
#endif
