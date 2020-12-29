#pragma once
#ifndef DSY_GRAINLET_H
#define DSY_GRAINLET_H

#include <stdint.h>
#ifdef __cplusplus

/** @file grainlet.h */
namespace daisysp
{
/**  
       Granular Oscillator Module \n 
	   A phase-distorted single cycle sine * another continuously running sine, \n
	   the whole thing synced to a main oscillator. \n
       Ported by Ben Sergentanis, Dec 2020 
       @author Emilie Gillet
       @date 2016
*/

class GrainletOscillator
{
  public:
    GrainletOscillator() {}
    ~GrainletOscillator() {}

    /** Initialize the oscillator
		\param sample_rate Sample rate of audio engine
	*/
    void Init(float sample_rate);

    /** Get the next sample */
    float Process();

    /** Sets the carrier frequency
		\param freq Frequency in Hz
	*/
    void SetCarrierFreq(float freq);

    /** Sets the formant frequency
		\param freq Frequency in Hz
	*/
    void SetFormantFreq(float freq);

    /** Sets the carrier waveshape
		\param shape Waveshape. Works best 0-1
	*/
    void SetShape(float shape);

    /** Sets the amount of formant to bleed through
		\param bleed Amount of bleed
	*/
    void SetBleed(float bleed);

  private:
    float Sine(float phase);

    float Carrier(float phase, float shape);

    float Grainlet(float carrier_phase,
                   float formant_phase,
                   float shape,
                   float bleed);

    float ThisBlepSample(float t);
    float NextBlepSample(float t);

    // Oscillator state.
    float carrier_phase_;
    float formant_phase_;
    float next_sample_;

    // For interpolation of parameters.
    float carrier_frequency_;
    float formant_frequency_;
    float carrier_shape_;
    float carrier_bleed_;

    float new_carrier_shape_;
    float new_carrier_bleed_;

    float sample_rate_;
};
} // namespace daisysp
#endif
#endif