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

    /** Initialize the module
	*/
    void Init();

    /** Reset the reverb */
    void Clear() { engine_.Clear(); }

    /** Get the next sample
		\param in Next sample to process
	*/
    float Process(float in);

    /** Set the reverb tail length
		\param rt Works 0-1.
	*/
    void SetTime(float rt) { rt_ = fclamp(rt, 0.f, 1.f); }

    /** Set the amount of reverb.
		\param amount Works 0-1.
	*/
    void SetAmount(float amount) { amount_ = fclamp(amount, 0.f, 1.f); };

  private:
    float rt_, amount_;

    uint16_t                              buffer_[8192];
    typedef FxEngine<8192, FORMAT_12_BIT> E;
    E                                     engine_;
    float                                 lp_decay_;
};
} // namespace daisysp
#endif
#endif