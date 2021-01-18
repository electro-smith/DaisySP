#include "modules/stringvoice.h"
#include <algorithm>
#include "modules/dust.h"
#include "modules/dsp.h"

using namespace daisysp;

void StringVoice::Init(float sample_rate) {
	sample_rate_ = sample_rate;

  excitation_filter_.Init(sample_rate);
  string_.Init(sample_rate_);
  remaining_noise_samples_ = 0;
}

void StringVoice::Reset() {
  string_.Reset();
}

void StringVoice::Render(
    bool sustain,
    bool trigger,
    float accent,
    float f0,
    float structure,
    float brightness,
    float damping,
    float* temp,
    float* out,
    float* aux,
    size_t size) {
  const float density = brightness * brightness;

  brightness += 0.25f * accent * (1.0f - brightness);
  damping += 0.25f * accent * (1.0f - damping);
  
  // Synthesize excitation signal.
  if (trigger || sustain) {
    const float range = 72.0f;
    const float f = 4.0f * f0;
    const float cutoff = fmin(
        f * SemitonesToRatio((brightness * (2.0f - brightness) - 0.5f) * range),
        0.499f);
    const float q = sustain ? 1.0f : 0.5f;
    remaining_noise_samples_ = static_cast<size_t>(1.0f / f0);
    excitation_filter_.set_f_q<FREQUENCY_DIRTY>(cutoff, q);
  }

  if (sustain) {
    const float dust_f = 0.00005f + 0.99995f * density * density;
    for (size_t i = 0; i < size; ++i) {
      temp[i] = Dust(dust_f) * (8.0f - dust_f * 6.0f) * accent;
    }
  } else if (remaining_noise_samples_) {
    size_t noise_samples = fmin(remaining_noise_samples_, size);
    remaining_noise_samples_ -= noise_samples;
    size_t tail = size - noise_samples;
    float* start = temp;
    while (noise_samples--) {
      *start++ = 2.0f * rand() * kRandFrac - 1.0f;
    }
    while (tail--) {
      *start++ = 0.0f;
    }
  } else {
    fill(&temp[0], &temp[size], 0.0f);
  }
  
  excitation_filter_.Process<FILTER_MODE_LOW_PASS>(temp, temp, size);
  for (size_t i = 0; i < size; ++i) {
    aux[i] += temp[i];
  }
  
  float non_linearity = structure < 0.24f
      ? (structure - 0.24f) * 4.166f
      : (structure > 0.26f ? (structure - 0.26f) * 1.35135f : 0.0f);
  string_.Process(f0, non_linearity, brightness, damping, temp, out, size);
}
