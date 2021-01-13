#pragma once
#ifndef DSY_HIHAT_H
#define DSY_HIHAT_H

#include "modules/svf.h"
#include "modules/oscillator.h"

#include <stdint.h>
#ifdef __cplusplus

/** @file hihat.h */

namespace daisysp
{
/**  
       @brief 808 style "metallic noise" with 6 square oscillators.
	   @author Ben Sergentanis
	   @date Jan 2021
	   Ported from pichenettes/eurorack/plaits/dsp/drums/hihat.h \n
	   to an independent module. \n
	   Original code written by Emilie Gillet in 2016. \n
*/
class SquareNoise
{
  public:
    SquareNoise() {}
    ~SquareNoise() {}

    void Init();

    float Process(float f0);

  private:
    uint32_t phase_[6];
};

/**  
       @brief Ring mod style metallic noise generator.
	   @author Ben Sergentanis
	   @date Jan 2021
	   Ported from pichenettes/eurorack/plaits/dsp/drums/hihat.h \n
	   to an independent module. \n
	   Original code written by Emilie Gillet in 2016. \n
*/
class RingModNoise
{
  public:
    RingModNoise() {}
    ~RingModNoise() {}

    void Init(float sample_rate);

    float Process(float f0);

  private:
    float      ProcessPair(Oscillator* osc, float f1, float f2);
    Oscillator oscillator_[6];

    float sample_rate_;
};

/**  
       @brief Swing type VCA
	   @author Ben Sergentanis
	   @date Jan 2021
	   Ported from pichenettes/eurorack/plaits/dsp/drums/hihat.h \n
	   to an independent module. \n
	   Original code written by Emilie Gillet in 2016. \n
*/
class SwingVCA
{
  public:
    float operator()(float s, float gain)
    {
        s *= s > 0.0f ? 10.0f : 0.1f;
        s = s / (1.0f + fabsf(s));
        return (s + 1.0f) * gain;
    }
};

/**  
       @brief Linear type VCA
	   @author Ben Sergentanis
	   @date Jan 2021
	   Ported from pichenettes/eurorack/plaits/dsp/drums/hihat.h \n
	   to an independent module. \n
	   Original code written by Emilie Gillet in 2016. \n
*/
class LinearVCA
{
  public:
    float operator()(float s, float gain) { return s * gain; }
};

/**  
       @brief 808 HH, with a few extra parameters to push things to the CY territory...
	   @author Ben Sergentanis
	   @date Jan 2021
	   The template parameter MetallicNoiseSource allows another kind of "metallic \n
	   noise" to be used, for results which are more similar to KR-55 or FM hi-hats. \n \n 
	   Ported from pichenettes/eurorack/plaits/dsp/drums/hihat.h \n
	   to an independent module. \n
	   Original code written by Emilie Gillet in 2016. \n
*/
template <typename MetallicNoiseSource = SquareNoise, typename VCA = LinearVCA, bool resonance = true>
class HiHat
{
  public:
    HiHat() {}
    ~HiHat() {}

    void Init(float sample_rate)
    {
        sample_rate_ = sample_rate;

        envelope_     = 0.0f;
        noise_clock_  = 0.0f;
        noise_sample_ = 0.0f;
        sustain_gain_ = 0.0f;

        metallic_noise_.Init(sample_rate_);
        noise_coloration_svf_.Init(sample_rate_);
        hpf_.Init(sample_rate_);
    }

    float Process(bool  sustain,
                  bool  trigger,
                  float accent,
                  float f0,
                  float tone,
                  float decay,
                  float noisiness)
    {
        const float envelope_decay
            = 1.0f - 0.003f * SemitonesToRatio(-decay * 84.0f);
        const float cut_decay
            = 1.0f - 0.0025f * SemitonesToRatio(-decay * 36.0f);

        if(trigger)
        {
            envelope_ = (1.5f + 0.5f * (1.0f - decay)) * (0.3f + 0.7f * accent);
        }

        // Process the metallic noise.
        float out = metallic_noise_.Process(2.0f * f0);

        // Apply BPF on the metallic noise.
        float cutoff = 150.0f / sample_rate_ * SemitonesToRatio(tone * 72.0f);

        cutoff = fclamp(cutoff, 0.0f, 16000.0f / sample_rate_);


        noise_coloration_svf_.SetFreq(cutoff * sample_rate_);
        noise_coloration_svf_.SetRes(resonance ? 3.0f + 6.0f * tone : 1.0f);

        noise_coloration_svf_.Process(out);
        out = noise_coloration_svf_.Band();

        // This is not at all part of the 808 circuit! But to add more variety, we
        // add a variable amount of clocked noise to the output of the 6 schmitt
        // trigger oscillators.
        noisiness *= noisiness;
        float noise_f = f0 * (16.0f + 16.0f * (1.0f - noisiness));
        noise_f       = fclamp(noise_f, 0.0f, 0.5f);

        noise_clock_ += noise_f;
        if(noise_clock_ >= 1.0f)
        {
            noise_clock_ -= 1.0f;
            noise_sample_ = random() * rand_frac_ - 0.5f;
        }
        out += noisiness * (noise_sample_ - out);

        // Apply VCA.
        sustain_gain_ = accent * decay;
        VCA vca;
        envelope_ *= envelope_ > 0.5f ? envelope_decay : cut_decay;
        out = vca(out, sustain ? sustain_gain_ : envelope_);

        hpf_.SetFreq(cutoff * sample_rate_);
        hpf_.SetRes(.5f);
        hpf_.Process(out);
        out = hpf_.High();

        return out;
    }

  private:
    float sample_rate_;

    float ratio_frac_ = 1.f / 12.f;
    float SemitonesToRatio(float in) { return powf(2.f, in * ratio_frac_); }

    float rand_frac_ = 1.f / (float)RAND_MAX;

    float envelope_;
    float noise_clock_;
    float noise_sample_;
    float sustain_gain_;

    MetallicNoiseSource metallic_noise_;
    Svf                 noise_coloration_svf_;
    Svf                 hpf_;
};
} // namespace daisysp
#endif
#endif