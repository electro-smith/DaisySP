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

    /** Initialize the module
		\param sample_rate Audio engine sample rate
	*/
    void Init(float sample_rate);
    
	/** Reset the string oscillator */
	void Reset();
	
	/** Get the next sample
		\param trigger Strike the resonator. Defaults to false.
	*/
    float Process(bool   trigger = false);

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
    float f0_, brightness_, damping_;
    float density_, accent_;
    float aux_;

    Dust   dust_;
    Svf    excitation_filter_;
    String string_;
    size_t remaining_noise_samples_;
};
} // namespace daisysp
#endif
#endif