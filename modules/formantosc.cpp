#include "dsp.h"
#include "paraminterp.h"
#include "resources.h"

#include "formantosc.h"
#include <math.h>

using namespace daisysp;

  void FormantOscillator::Init(float sample_rate) {
    carrier_phase_ = 0.0f;
    formant_phase_ = 0.0f;
    next_sample_ = 0.0f;
  
    carrier_frequency_ = 0.0f;
    formant_frequency_ = 100.f;
    phase_shift_ = 0.0f;
	
	sample_rate_ = sample_rate;
  }
  
  float FormantOscillator::Process(float carrier_frequency, float formant_frequency, float phase_shift) {
	//convert from Hz to phase_inc / sample
	carrier_frequency = carrier_frequency / sample_rate_;
	formant_frequency = formant_frequency / sample_rate_;
	
	float kMaxFrequency = .25f;
	if (carrier_frequency >= kMaxFrequency) {
      carrier_frequency = kMaxFrequency;
    }
    if (formant_frequency >= kMaxFrequency) {
      formant_frequency = kMaxFrequency;
    }

    ParameterInterpolator carrier_fm( &carrier_frequency_, carrier_frequency, (size_t)1);
    ParameterInterpolator formant_fm( &formant_frequency_, formant_frequency, (size_t)1);
    ParameterInterpolator pm(&phase_shift_, phase_shift, (size_t)1);

    float next_sample = next_sample_;
    
      float this_sample = next_sample;
      next_sample = 0.0f;
    
      carrier_frequency = carrier_fm.Next();
      formant_frequency = formant_fm.Next();
    
      carrier_phase_ += carrier_frequency;
      
      if (carrier_phase_ >= 1.0f) {
        carrier_phase_ -= 1.0f;
        float reset_time = carrier_phase_ / carrier_frequency;
    
        float formant_phase_at_reset = formant_phase_ + \
            (1.0f - reset_time) * formant_frequency;
        float before = Sine(
            formant_phase_at_reset + pm.subsample(1.0f - reset_time));
        float after = Sine(0.0f + pm.subsample(1.0f));
        float discontinuity = after - before;
        this_sample += discontinuity * ThisBlepSample(reset_time);
        next_sample += discontinuity * NextBlepSample(reset_time);
        formant_phase_ = reset_time * formant_frequency;
      } 
	  else {
        formant_phase_ += formant_frequency;
        if (formant_phase_ >= 1.0f) {
          formant_phase_ -= 1.0f;
      }
     }
    
      phase_shift = pm.Next();
      next_sample += Sine(formant_phase_ + phase_shift);
	
    
    next_sample_ = next_sample;
	return this_sample;
  
  }

  inline float FormantOscillator::Sine(float phase) {
    return InterpolateWrap(lut_sine, phase, 1024.0f);
  }
  
  inline float FormantOscillator::ThisBlepSample(float t) {
    return 0.5f * t * t;
  }
  
  inline float FormantOscillator::NextBlepSample(float t) {
	t = 1.0f - t;
    return -0.5f * t * t;
  }

  //DISALLOW_COPY_AND_ASSIGN(FormantOscillator);