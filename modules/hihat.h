#pragma once
#ifndef DSY_HIHAT_H
#define DSY_HIHAT_H

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

    void
    Process(float f0, float* temp_1, float* temp_2, float* out, size_t size);

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

    void Init();

    void
    Process(float f0, float* temp_1, float* temp_2, float* out, size_t size);

  private:
    void       ProcessPair(Oscillator* osc,
                           float       f1,
                           float       f2,
                           float*      temp_1,
                           float*      temp_2,
                           float*      out,
                           size_t      size);
    Oscillator oscillator_[6];
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
template <typename MetallicNoiseSource, typename VCA, bool resonance>
class HiHat
{
  public:
    HiHat() {}
    ~HiHat() {}

    void Init()
    {
        envelope_     = 0.0f;
        noise_clock_  = 0.0f;
        noise_sample_ = 0.0f;
        sustain_gain_ = 0.0f;

        metallic_noise_.Init();
        noise_coloration_svf_.Init();
        hpf_.Init();
    }

    void Process(bool   sustain,
                 bool   trigger,
                 float  accent,
                 float  f0,
                 float  tone,
                 float  decay,
                 float  noisiness,
                 float* temp_1,
                 float* temp_2,
                 float* out,
                 size_t size)
    {
        const float envelope_decay
            = 1.0f - 0.003f * stmlib::SemitonesToRatio(-decay * 84.0f);
        const float cut_decay
            = 1.0f - 0.0025f * stmlib::SemitonesToRatio(-decay * 36.0f);

        if(trigger)
        {
            envelope_ = (1.5f + 0.5f * (1.0f - decay)) * (0.3f + 0.7f * accent);
        }

        // Process the metallic noise.
        metallic_noise_.Process(2.0f * f0, temp_1, temp_2, out, size);

        // Apply BPF on the metallic noise.
        float cutoff
            = 150.0f / kSampleRate * stmlib::SemitonesToRatio(tone * 72.0f);
        CONSTRAIN(cutoff, 0.0f, 16000.0f / kSampleRate);
        noise_coloration_svf_.set_f_q<stmlib::FREQUENCY_ACCURATE>(
            cutoff, resonance ? 3.0f + 6.0f * tone : 1.0f);
        noise_coloration_svf_.Process<stmlib::FILTER_MODE_BAND_PASS>(
            out, out, size);

        // This is not at all part of the 808 circuit! But to add more variety, we
        // add a variable amount of clocked noise to the output of the 6 schmitt
        // trigger oscillators.
        noisiness *= noisiness;
        float noise_f = f0 * (16.0f + 16.0f * (1.0f - noisiness));
        CONSTRAIN(noise_f, 0.0f, 0.5f);

        for(size_t i = 0; i < size; ++i)
        {
            noise_clock_ += noise_f;
            if(noise_clock_ >= 1.0f)
            {
                noise_clock_ -= 1.0f;
                noise_sample_ = stmlib::Random::GetFloat() - 0.5f;
            }
            out[i] += noisiness * (noise_sample_ - out[i]);
        }

        // Apply VCA.
        stmlib::ParameterInterpolator sustain_gain(
            &sustain_gain_, accent * decay, size);
        for(size_t i = 0; i < size; ++i)
        {
            VCA vca;
            envelope_ *= envelope_ > 0.5f ? envelope_decay : cut_decay;
            out[i] = vca(out[i], sustain ? sustain_gain.Next() : envelope_);
        }

        hpf_.set_f_q<stmlib::FREQUENCY_ACCURATE>(cutoff, 0.5f);
        hpf_.Process<stmlib::FILTER_MODE_HIGH_PASS>(out, out, size);
    }

  private:
    float envelope_;
    float noise_clock_;
    float noise_sample_;
    float sustain_gain_;

    MetallicNoiseSource metallic_noise_;
    stmlib::Svf         noise_coloration_svf_;
    stmlib::Svf         hpf_;
};
} // namespace daisysp
#endif
#endif