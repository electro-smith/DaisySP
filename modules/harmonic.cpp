#include "dsp.h"
#include "harmonic.h"

using namespace daisysp;

  void HarmonicOscillator::Init(int num_harmonics) {
    phase_ = 0.0f;
    frequency_ = 0.0f;
	num_harmonics_ = num_harmonics > 32 ? 32 : num_harmonics;
    for (int i = 0; i < num_harmonics_; ++i) {
      amplitude_[i] = 0.0f;
    }
  }
  
  void HarmonicOscillator::Process(
	int first_harmonic_index, 
  float frequency,
      const float* amplitudes,
      float* out,
      size_t size) {
    if (frequency >= 0.5f) {
      frequency = 0.5f;
    }
    
    stmlib::ParameterInterpolator am[num_harmonics_];
    stmlib::ParameterInterpolator fm(&frequency_, frequency, size);
    
    for (int i = 0; i < num_harmonics_; ++i) {
      float f = frequency * static_cast<float>(first_harmonic_index + i);
      if (f >= 0.5f) {
        f = 0.5f;
      }
      am[i].Init(&amplitude_[i], amplitudes[i] * (1.0f - f * 2.0f), size);
    }

    while (size--) {
      phase_ += fm.Next();
      if (phase_ >= 1.0f) {
        phase_ -= 1.0f;
      }
      const float two_x = 2.0f * stmlib::Interpolate(lut_sine, phase_, 1024.0f);
      float previous, current;
      if (first_harmonic_index == 1) {
        previous = 1.0f;
        current = two_x * 0.5f;
      } else {
        const float k = first_harmonic_index;
        previous = stmlib::InterpolateWrap(
            lut_sine, phase_ * (k - 1.0f) + 0.25f, 1024.0f);
        current = stmlib::InterpolateWrap(lut_sine, phase_ * k, 1024.0f);
      }
      
      float sum = 0.0f;
      for (int i = 0; i < num_harmonics_; ++i) {
        sum += am[i].Next() * current;
        float temp = current;
        current = two_x * current - previous;
        previous = temp;
      }
      if (first_harmonic_index == 1) {
        *out++ = sum;
      } else {
        *out++ += sum;
      }
    }
	  }