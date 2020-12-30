#pragma once
#ifndef DSY_VOSIM_H
#define DSY_VOSIM_H

#include <stdint.h>
#ifdef __cplusplus

/** @file vosim.h */

namespace daisysp
{
/**  
       Vosim Oscillator Module \n 
	   Two sinewaves multiplied by and sync'ed to a carrier. \n
       Ported by Ben Sergentanis, Dec 2020 
       @author Emilie Gillet
       @date 2016
*/
class VOSIMOscillator
{
  public:
    VOSIMOscillator() {}
    ~VOSIMOscillator() {}

    /** Initializes the FormantOscillator module.
        \param sample_rate - The sample rate of the audio engine being run. 
    */
    void Init(float sample_rate);

    /** Get the next sample
    */
    float Process();

    /** Set carrier frequency.
		\param freq Frequency in Hz.
	*/
    void SetCarrierFreq(float freq);

    /** Set formant 1 frequency.
		\param freq Frequency in Hz.
	*/
    void SetForm1Freq(float freq);

    /** Set formant 2 frequency.
		\param freq Frequency in Hz.
	*/
    void SetForm2Freq(float freq);

    /** Set carrier waveshape
		\param shape Shape to set.
	*/
    void SetShape(float shape);

  private:
    float Sine(float phase);

    float sample_rate_;

    // Oscillator state.
    float carrier_phase_;
    float formant_1_phase_;
    float formant_2_phase_;

    // For interpolation of parameters.
    float carrier_frequency_;
    float formant_1_frequency_;
    float formant_2_frequency_;
    float carrier_shape_;
};
} // namespace daisysp
#endif
#endif