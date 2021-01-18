#pragma once
#ifndef DSY_STRINGVOICE_H
#define DSY_STRINGVOICE_H

#include "modules/svf.h"
#include "modules/string.h"
#include "modules/dust.h"
#include <stdint.h>
#ifdef __cplusplus

/** @file stringvoice.h */

namespace daisysp
{
/**  
       @brief Extended Karplus-Strong, with all the niceties from Rings 
	   @author Ben Sergentanis
	   @date Jan 2021
	   Ported from pichenettes/eurorack/plaits/dsp/physical_modelling/string_voice.h \n
	   and pichenettes/eurorack/plaits/dsp/physical_modelling/string_voice.cc \n
	   to an independent module. \n
	   Original code written by Emilie Gillet in 2016. \n
*/
class StringVoice
{
  public:
    StringVoice() {}
    ~StringVoice() {}

    void Init(float sample_rate);
    void Reset();
    void Render(bool   sustain,
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

    Dust   dust_;
    Svf    excitation_filter_;
    String string_;
    size_t remaining_noise_samples_;
};
} // namespace daisysp
#endif
#endif