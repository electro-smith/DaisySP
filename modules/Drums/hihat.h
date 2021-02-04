#pragma once
#ifndef DSY_HIHAT_H
#define DSY_HIHAT_H

#include "Filters/svf.h"
#include "Synthesis/oscillator.h"

#include <stdint.h>
#include <stdlib.h>
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

    void Init(float sample_rate);

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
template <typename MetallicNoiseSource = SquareNoise,
          typename VCA                 = LinearVCA,
          bool resonance               = true>
class HiHat
{
  public:
    HiHat() {}
    ~HiHat() {}

    /** Initialize the module
        \param sample_rate Audio engine sample rate
    */
    void Init(float sample_rate)
    {
        sample_rate_ = sample_rate;

        trig_ = false;

        envelope_     = 0.0f;
        noise_clock_  = 0.0f;
        noise_sample_ = 0.0f;
        sustain_gain_ = 0.0f;

        SetFreq(3000.f);
        SetTone(.5f);
        SetDecay(.2f);
        SetNoisiness(.8f);
        SetAccent(.8f);
        SetSustain(false);

        metallic_noise_.Init(sample_rate_);
        noise_coloration_svf_.Init(sample_rate_);
        hpf_.Init(sample_rate_);
    }

    /** Get the next sample
        \param trigger Hit the hihat with true. Defaults to false.
    */
    float Process(bool trigger = false)
    {
        const float envelope_decay
            = 1.0f - 0.003f * SemitonesToRatio(-decay_ * 84.0f);
        const float cut_decay
            = 1.0f - 0.0025f * SemitonesToRatio(-decay_ * 36.0f);

        if(trigger || trig_)
        {
            trig_ = false;

            envelope_
                = (1.5f + 0.5f * (1.0f - decay_)) * (0.3f + 0.7f * accent_);
        }

        // Process the metallic noise.
        float out = metallic_noise_.Process(2.0f * f0_);

        // Apply BPF on the metallic noise.
        float cutoff = 150.0f / sample_rate_ * SemitonesToRatio(tone_ * 72.0f);

        cutoff = fclamp(cutoff, 0.0f, 16000.0f / sample_rate_);


        noise_coloration_svf_.SetFreq(cutoff * sample_rate_);
        noise_coloration_svf_.SetRes(resonance ? 3.0f + 6.0f * tone_ : 1.0f);

        noise_coloration_svf_.Process(out);
        out = noise_coloration_svf_.Band();

        // This is not at all part of the 808 circuit! But to add more variety, we
        // add a variable amount of clocked noise to the output of the 6 schmitt
        // trigger oscillators.
        float noise_f = f0_ * (16.0f + 16.0f * (1.0f - noisiness_));
        noise_f       = fclamp(noise_f, 0.0f, 0.5f);

        noise_clock_ += noise_f;
        if(noise_clock_ >= 1.0f)
        {
            noise_clock_ -= 1.0f;
            noise_sample_ = rand() * kRandFrac - 0.5f;
        }
        out += noisiness_ * (noise_sample_ - out);

        // Apply VCA.
        sustain_gain_ = accent_ * decay_;
        VCA vca;
        envelope_ *= envelope_ > 0.5f ? envelope_decay : cut_decay;
        out = vca(out, sustain_ ? sustain_gain_ : envelope_);

        hpf_.SetFreq(cutoff * sample_rate_);
        hpf_.SetRes(.5f);
        hpf_.Process(out);
        out = hpf_.High();

        return out;
    }

    /** Trigger the hihat */
    void Trig() { trig_ = true; }

    /** Make the hihat ring out infinitely.
        \param sustain True = infinite sustain.
    */
    void SetSustain(bool sustain) { sustain_ = sustain; }

    /** Set how much accent to use
        \param accent Works 0-1.
    */
    void SetAccent(float accent) { accent_ = fclamp(accent, 0.f, 1.f); }

    /** Set the hihat tone's root frequency
        \param f0 Freq in Hz
    */
    void SetFreq(float f0)
    {
        f0 /= sample_rate_;
        f0_ = fclamp(f0, 0.f, 1.f);
    }

    /** Set the overall brightness / darkness of the hihat.
        \param tone Works from 0-1.
    */
    void SetTone(float tone) { tone_ = fclamp(tone, 0.f, 1.f); }

    /** Set the length of the hihat decay
        \param decay Works > 0. Tuned for 0-1.
    */
    void SetDecay(float decay)
    {
        decay_ = fmax(decay, 0.f);
        decay_ *= 1.7;
        decay_ -= 1.2;
    }

    /** Sets the mix between tone and noise
        \param snappy 1 = just noise. 0 = just tone.
    */
    void SetNoisiness(float noisiness)
    {
        noisiness_ = fclamp(noisiness, 0.f, 1.f);
        noisiness_ *= noisiness_;
    }


  private:
    float sample_rate_;

    float accent_, f0_, tone_, decay_, noisiness_;
    bool  sustain_;
    bool  trig_;

    float SemitonesToRatio(float in) { return powf(2.f, in * kOneTwelfth); }

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