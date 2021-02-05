#pragma once
#ifndef DSY_ZOSCILLATOR_H
#define DSY_ZOSCILLATOR_H

#include <stdint.h>
#ifdef __cplusplus

/** @file zoscillator.h */
namespace daisysp
{
/**  
    @brief     ZOscillator Module \n 
    @author Ben Sergentanis
    @date Dec 2020 
    Sinewave multiplied by and sync'ed to a carrier. \n \n
    Ported from pichenettes/eurorack/plaits/dsp/oscillator/z_oscillator.h \n 
    \n to an independent module. \n
    Original code written by Emilie Gillet in 2016. \n
*/
class ZOscillator
{
  public:
    ZOscillator() {}
    ~ZOscillator() {}

    /** Init ZOscillator module
        \param sample_rate Audio engine sample rate.
    */
    void Init(float sample_rate);

    /** Get next sample
    */
    float Process();

    /** Set the carrier frequency
        \param freq Frequency in Hz.
    */
    void SetFreq(float freq);

    /** Set the formant osc. freq
        \param freq Frequency in Hz.
    */
    void SetFormantFreq(float freq);

    /** Adjust the contour of the waveform.
        \param shape Waveshape to set. Works best 0-1.
    */
    void SetShape(float shape);

    /** Set the offset amount and phase shift. \n
        < 1/3 is just phase shift, > 2/3 is just offset, and between them is both. \n
        \param mode Mode to set. Works best -1 to 1
    */
    void SetMode(float mode);

  private:
    inline float Sine(float phase);
    inline float Z(float c, float d, float f, float shape, float mode);

    float sample_rate_;

    // Oscillator state.
    float carrier_phase_;
    float discontinuity_phase_;
    float formant_phase_;
    float next_sample_;

    // For interpolation of parameters.
    float carrier_frequency_;
    float formant_frequency_;
    float carrier_shape_, shape_new_;
    float mode_, mode_new_;
};
} // namespace daisysp
#endif
#endif