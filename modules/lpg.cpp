#include "dsp.h"
#include "lpg.h"
#include <math.h>

using namespace daisysp;

  void LowPassGate::Init(float sample_rate) {
    previous_gain_ = 0.0f;
    filter_.Init(sample_rate);
	sample_rate_ = sample_rate;
  }
  
  void LowPassGate::Process(
      float gain,
      float frequency,
      float hf_bleed,
      float* in_out,
      size_t size) {
    //filter_.set_f_q<stmlib::FREQUENCY_DIRTY>(frequency, 0.4f);
    filter_.SetFreq(frequency);
	while (size--) {
      const float s = *in_out * gain;
      filter_.Process(s);
      const float lp = filter_.Low();
	  *in_out++ = lp + (s - lp) * hf_bleed;
    }
  }
  
  void LowPassGate::Process(
      float gain,
      float frequency,
      float hf_bleed,
      float* in,
      short* out,
      size_t size,
      size_t stride) {
    filter_.SetFreq(frequency);
	//filter_.set_f_q<stmlib::FREQUENCY_DIRTY>(frequency, 0.4f);
    while (size--) {
      const float s = *in++ * gain;
	  filter_.Process(s);
      const float lp = filter_.Low();
      *out = Clip16(1 + static_cast<int32_t>(lp + (s - lp) * hf_bleed));
      out += stride;
    }
  }