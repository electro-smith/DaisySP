  #include "dsp.h"
#include "wavetable.h"
#include <math.h>

using namespace daisysp;
  void Differentiator::Init() {
    previous_ = 0.0f;
    lp_ = 0.0f;
  }
  
  float Differentiator::Process(float coefficient, float s) {
    ONE_POLE(lp_, s - previous_, coefficient);
    previous_ = s;
    return lp_;
  }


  void WavetableOscillator::Init(float sample_rate) {
	sample_rate_ = sample_rate;

    phase_ = 0.0f;
    frequency_ = 0.0f;
    amplitude_ = 0.0f;
    waveform_ = 0.0f;
    lp_ = 0.0f;
    differentiator_.Init();
  }
  
  float WavetableOscillator::Process(
      float frequency,
      float amplitude,
      float waveform,
      const int16_t** wavetable){
		  if (frequency >= kMaxFrequency) {
      frequency = kMaxFrequency;
    }
    amplitude *= 1.0f - 2.0f * frequency;
    if (approximate_scale) {
      amplitude *= 1.0f / (frequency * 131072.0f) * (0.95f - frequency);
    }

    stmlib::ParameterInterpolator frequency_modulation(
        &frequency_,
        frequency,
        size);
    stmlib::ParameterInterpolator amplitude_modulation(
        &amplitude_,
        amplitude,
        size);
    stmlib::ParameterInterpolator waveform_modulation(
        &waveform_,
        waveform * float(num_waves - 1.0001f),
        size);
    
    float lp = lp_;
    float phase = phase_;
	
	//while
      const float f0 = frequency_modulation.Next();
      const float cutoff = std::min(float(wavetable_size) * f0, 1.0f);
      
      const float scale = approximate_scale ? 1.0f : 1.0f / (f0 * 131072.0f) * (0.95f - f0);
      
      phase += f0;
      if (phase >= 1.0f) {
        phase -= 1.0f;
      }
      
      const float waveform = waveform_modulation.Next();
      MAKE_INTEGRAL_FRACTIONAL(waveform);
      
      const float p = phase * float(wavetable_size);
      MAKE_INTEGRAL_FRACTIONAL(p);
      
      const float x0 = InterpolateWave(
          wavetable[waveform_integral], p_integral, p_fractional);
      const float x1 = InterpolateWave(
          wavetable[waveform_integral + 1], p_integral, p_fractional);
      
      const float s = differentiator_.Process(
          cutoff,
          x0 + (x1 - x0) * waveform_fractional);
      ONE_POLE(lp, s * scale, cutoff * 0.5f);
     //while
	
	lp_ = lp;
    phase_ = phase;
	
	return amplitude_modulation.Next() * lp;
  }