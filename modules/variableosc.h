// Continuously variable waveform: triangle > saw > square. Both square and
// triangle have variable slope / pulse-width. Additionally, the phase resets
// can be locked to a master frequency.

#pragma once
#ifndef DSY_VARIABLEOSCILLATOR_H
#define DSY_VARIABLEOSCILLATOR_H

#include <stdint.h>
#ifdef __cplusplus

/** @file variableosc.h */

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

    void Init(float sample_rate);

    float Process();

    void SetSync(bool enable_sync);

    void SetFreq(float frequency);

    void SetSyncFreq(float frequency);

    void SetPW(float pw);

    void SetWaveshape(float waveshape);

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