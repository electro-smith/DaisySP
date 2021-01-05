#pragma once
#ifndef DSY_VARIABLESHAPEOSCILLATOR_H
#define DSY_VARIABLESHAPEOSCILLATOR_H

#include <stdint.h>
#ifdef __cplusplus

/** @file variableshapeosc.h */

namespace daisysp
{
/**  
       Variable Waveshape Oscillator \n 
	   Continuously variable waveform. \n
       Ported by Ben Sergentanis, Dec 2020 
       @author Emilie Gillet
       @date 2016
*/
class VariableShapeOscillator
{
  public:
    VariableShapeOscillator() {}
    ~VariableShapeOscillator() {}

    /** Initialize the oscillator
		\param sample_rate Audio engine sample rate
	*/
    void Init(float sample_rate);

    /** Get next sample
	*/
    float Process();

    /** Set master freq.
		\param frequency Freq in Hz.
	*/
    void SetFreq(float frequency);

    /** Set pulse width / saw, ramp, tri.
		\param pw PW when shape is square. Saw, ramp, tri otherwise.
	*/
    void SetPW(float pw);

    /** Switch from saw/ramp/tri to square.
		\param waveshape 0 is saw/ramp/tri, 1 is square.
	*/
    void SetWaveshape(float waveshape);

    /** Whether or not to sync to the sync oscillator
		\param enable_sync True to turn sync on.
	*/
    void SetSync(bool enable_sync);

    /** Set sync oscillator freq.
		\param frequency Freq in Hz.
	*/
    void SetSyncFreq(float frequency);

  private:
    float ComputeNaiveSample(float phase,
                             float pw,
                             float slope_up,
                             float slope_down,
                             float triangle_amount,
                             float square_amount);

    float ThisBlepSample(float t);

    float NextBlepSample(float t);

    float NextIntegratedBlepSample(float t);

    float ThisIntegratedBlepSample(float t);

    float sample_rate_;
    bool  enable_sync_;

    // Oscillator state.
    float master_phase_;
    float slave_phase_;
    float next_sample_;
    float previous_pw_;
    bool  high_;

    // For interpolation of parameters.
    float master_frequency_;
    float slave_frequency_;
    float pw_;
    float waveshape_;
};
} // namespace daisysp
#endif
#endif