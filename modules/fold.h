// #fold
// fold module
// ## Credits
// *Original author(s) : John FFitch, Gabriel Maldonado
// *Year : 1998
// *Ported from soundpipe by Ben Sergentanis, May 2020

#pragma once
#ifndef DSY_FOLD_H
#define DSY_FOLD_H

#include <stdint.h>
#ifdef __cplusplus

namespace daisysp
{

class Fold
{
    public:
    Fold() {}
    ~Fold() {}
  
    // ### Init
    //
    // Initializes the fold module.
    // ~~~~
        void Init();
    // ~~~~

    // ### Process
    //
    // ~~~~
        float Process(float in);
    // ~~~~

    // #Setters
    //
    // ###SetIncrement
    //
    // * float incr : set fold increment
    // ~~~~
        inline void SetIncrement(float incr) { incr_ = incr; }
    // ~~~~

    private:
	float  incr_, index_, value_;
	int sample_index_;
    };
} // namespace daisysp
#endif
#endif
