#pragma once
#ifndef DSY_DIFFUSER_H
#define DSY_DIFFUSER_H

#include "modules/fx_engine.h"

#include <stdint.h>
#ifdef __cplusplus

/** @file diffuser.h */

namespace daisysp
{
/**  
       @brief Granular Diffuser
	   @author Ben Sergentanis
	   @date Jan 2021
	   Ported from pichenettes/eurorack/plaits/dsp/fx/diffuser.h \n
	   to an independent module. \n
	   Original code written by Emilie Gillet in 2016. \n
*/
class Diffuser
{
  public:
    Diffuser() {}
    ~Diffuser() {}

    void Init(uint16_t* buffer);

    void Clear() { engine_.Clear(); }

    void Process(float amount, float rt, float* in_out, size_t size);
	
  private:
    typedef FxEngine<8192, FORMAT_12_BIT> E;
    E                                     engine_;
    float                                 lp_decay_;
};
} // namespace daisysp
#endif
#endif