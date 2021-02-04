#pragma once
#ifndef DSY_GRAINLET_H
#define DSY_GRAINLET_H

#include <stdint.h>
#ifdef __cplusplus

/** @file grainlet.h */
namespace daisysp
{
/**  
       @brief Granular Oscillator Module. 
       @author Ben Sergentanis
       @date Dec 2020 
       A phase-distorted single cycle sine * another continuously running sine, \n
       the whole thing synced to a main oscillator. \n \n
       Ported from pichenettes/eurorack/plaits/dsp/oscillator/grainlet_oscillator.h \n 
       to an independent module. \n
       Original code written by Emilie Gillet in 2016. \n
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
    void SetFreq(float freq);

    /** Sets the formant frequency
        \param freq Frequency in Hz
    */
    void SetFormantFreq(float freq);

    /** Waveshaping
        \param shape Shapes differently from 0-1, 1-2, and > 2.
    */
    void SetShape(float shape);

    /** Sets the amount of formant to bleed through
        \param bleed Works best 0-1
    */
    void SetBleed(float bleed);

  private:
    float Sine(float phase);

    float Carrier(float phase, float shape);

    float Grainlet(float carrier_phase,
                   float formant_phase,
                   float shape,
                   float bleed);

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