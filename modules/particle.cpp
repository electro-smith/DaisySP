#include "dsp.h"
#include "particle.h"
#include <math.h>

using namespace daisysp;

  inline void Particle::Init(float sample_rate) {
    sample_rate_ = sample_rate;
	pre_gain_ = 0.0f;
    filter_.Init();
  }
  
  inline float Particle::Process(
      bool sync,
      float density,
      float gain,
      float frequency,
      float spread,
      float q,
      float* out,
      float* aux,
      size_t size) {
    float u = stmlib::Random::GetFloat();
    if (sync) {
      u = density;
    }
    bool can_radomize_frequency = true;
    while (size--) {
      float s = 0.0f;
      if (u <= density) {
        s = u * gain;
        if (can_radomize_frequency) {
          const float u = 2.0f * stmlib::Random::GetFloat() - 1.0f;
          const float f = std::min(
              stmlib::SemitonesToRatio(spread * u) * frequency,
              0.25f);
          pre_gain_ = 0.5f / stmlib::Sqrt(q * f * stmlib::Sqrt(density));
          filter_.set_f_q<stmlib::FREQUENCY_DIRTY>(f, q);
          // Keep the cutoff constant for this whole block.
          can_radomize_frequency = false;
        }
      }
      *aux++ += s;
      *out++ += filter_.Process<stmlib::FILTER_MODE_BAND_PASS>(pre_gain_ * s);
      u = GetFloat();
    }
  }