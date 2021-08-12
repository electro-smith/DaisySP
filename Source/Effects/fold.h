#pragma once
#ifndef DSY_FOLD_H
#define DSY_FOLD_H

#include <stdint.h>
#ifdef __cplusplus

namespace daisysp
{
/** fold module

Original author(s) : John FFitch, Gabriel Maldonado

Year : 1998

Ported from soundpipe by Ben Sergentanis, May 2020
*/
class Fold
{
  public:
    Fold() {}
    ~Fold() {}
    /** Initializes the fold module.
    */
    void Init();


    /** applies foldover distortion to input 
    */
    float Process(float in);


    /** 
        \param incr : set fold increment
    */
    inline void SetIncrement(float incr) { incr_ = incr; }

  private:
    float incr_, index_, value_;
    int   sample_index_;
};
} // namespace daisysp
#endif
#endif
