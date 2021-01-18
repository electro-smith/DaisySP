#pragma once
#ifndef DSY_MODAL_H
#define DSY_MODAL_H

#include <stdint.h>
#include "modules/svf.h"
#include "modules/resonator.h"
#include "modules/dust.h"
#ifdef __cplusplus

/** @file modalvoice.h */

namespace daisysp
{
/**  
       @brief Simple modal synthesis voice with a mallet exciter: click -> LPF -> resonator.
	   @author Ben Sergentanis
	   @date Jan 2021
	   The click can be replaced by continuous white noise. \n \n 
	   Ported from pichenettes/eurorack/plaits/dsp/physical_modelling/modal_voice.h \n
	   and pichenettes/eurorack/plaits/dsp/physical_modelling/modal_voice.cc \n 
	   to an independent module. \n
	   Original code written by Emilie Gillet in 2016. \n
*/
class ModalVoice
{
  public:
    ModalVoice() {}
    ~ModalVoice() {}

    void Init(float sample_rate);
    void Process(bool   sustain,
                 bool   trigger,
                 float  accent,
                 float  f0,
                 float  structure,
                 float  brightness,
                 float  damping,
                 float* temp,
                 float* out,
                 float* aux,
                 size_t size);

  private:
    float sample_rate_;

    ResonatorSvf<1> excitation_filter_;
    Resonator       resonator_;
    Dust            dust_;
};
} // namespace daisysp
#endif
#endif