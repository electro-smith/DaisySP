/*
Copyright (c) 2020 Electrosmith, Corp, Emilie Gillet

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/

#pragma once
#ifndef DSY_MODAL_H
#define DSY_MODAL_H

#include <stdint.h>
#include "Filters/svf.h"
#include "PhysicalModeling/resonator.h"
#include "Noise/dust.h"
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

    /** Initialize the module
        \param sample_rate Audio engine sample rate
    */
    void Init(float sample_rate);

    /** Get the next sample
        \param trigger Strike the resonator. Defaults to false.
    */
    float Process(bool trigger = false);

    /** Continually excite the resonator with noise.
        \param sustain True turns on the noise.
    */
    void SetSustain(bool sustain);

    /** Strike the resonator. */
    void Trig();

    /** Set the resonator root frequency.
        \param freq Frequency in Hz.
    */
    void SetFreq(float freq);

    /** Hit the resonator a bit harder.
        \param accent Works 0-1.
    */
    void SetAccent(float accent);

    /** Changes the general charater of the resonator (stiffness, brightness)
        \param structure Works best from 0-1
    */
    void SetStructure(float structure);

    /** Set the brighness of the resonator, and the noise density.
        \param brightness Works best 0-1
    */
    void SetBrightness(float brightness);

    /** How long the resonant body takes to decay.
        \param damping Works best 0-1
    */
    void SetDamping(float damping);

    /** Get the raw excitation signal. Must call Process() first. */
    float GetAux();

  private:
    float sample_rate_;

    bool  sustain_, trig_;
    float f0_, structure_, brightness_, damping_;
    float density_, accent_;
    float aux_;

    ResonatorSvf<1> excitation_filter_;
    Resonator       resonator_;
    Dust            dust_;
};
} // namespace daisysp
#endif
#endif