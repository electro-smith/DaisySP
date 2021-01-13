#pragma once
#ifndef DSY_SYNTHBD_H
#define DSY_SYNTHBD_H

#include "modules/svf.h"
#include "modules/dsp.h"

#include <stdint.h>
#ifdef __cplusplus

/** @file synthbassdrum.h */

namespace daisysp
{
/**  
       @brief Click noise for SyntheticBassDrum
	   @author Ben Sergentanis
	   @date Jan 2021
	   Ported from pichenettes/eurorack/plaits/dsp/drums/synthetic_bass_drum.h \n
	   to an independent module. \n
	   Original code written by Emilie Gillet in 2016. \n
*/
class SyntheticBassDrumClick
{
  public:
    SyntheticBassDrumClick() {}
    ~SyntheticBassDrumClick() {}

    void Init(float sample_rate);

    float Process(float in);

  private:
    float lp_;
    float hp_;
    Svf   filter_;
};

/**  
       @brief Attack Noise generator for SyntheticBassDrum. 
	   @author Ben Sergentanis
	   @date Jan 2021
	   Ported from pichenettes/eurorack/plaits/dsp/drums/synthetic_bass_drum.h \n
	   to an independent module. \n
	   Original code written by Emilie Gillet in 2016. \n
*/
class SyntheticBassDrumAttackNoise
{
  public:
    SyntheticBassDrumAttackNoise() {}
    ~SyntheticBassDrumAttackNoise() {}

    void Init();

    float Render();

  private:
  	float rand_frac_ = 1.f / (float)RAND_MAX;
  
    float lp_;
    float hp_;
};

/**  
       @brief Naive bass drum model (modulated oscillator with FM + envelope).
	   @author Ben Sergentanis
	   @date Jan 2021
	   Inadvertently 909-ish. \n \n 
	   Ported from pichenettes/eurorack/plaits/dsp/drums/synthetic_bass_drum.h \n
	   to an independent module. \n
	   Original code written by Emilie Gillet in 2016. \n
*/
class SyntheticBassDrum
{
  public:
    SyntheticBassDrum() {}
    ~SyntheticBassDrum() {}

    void Init(float sample_rate);

    inline float DistortedSine(float phase, float phase_noise, float dirtiness);

    inline float TransistorVCA(float s, float gain);

    void Render(bool   sustain,
                bool   trigger,
                float  accent,
                float  f0,
                float  tone,
                float  decay,
                float  dirtiness,
                float  fm_envelope_amount,
                float  fm_envelope_decay,
                float* out,
                size_t size);

  private:
	float sample_rate_;
  
    float f0_;
    float phase_;
    float phase_noise_;

    float fm_;
    float fm_lp_;
    float body_env_;
    float body_env_lp_;
    float transient_env_;
    float transient_env_lp_;

    float sustain_gain_;

    float tone_lp_;

	float ratio_frac_ = 1.f / 12.f;
	float rand_frac_ = 1.f / (float)RAND_MAX;

    SyntheticBassDrumClick       click_;
    SyntheticBassDrumAttackNoise noise_;

    int body_env_pulse_width_;
    int fm_pulse_width_;
};

} // namespace daisysp
#endif
#endif