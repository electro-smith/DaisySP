#include "synthbassdrum.h"
#include <math.h>

using namespace daisysp;

void SyntheticBassDrumClick::Init(float sample_rate)
    {
        lp_ = 0.0f;
        hp_ = 0.0f;
        filter_.Init(sample_rate);
		filter_.SetFreq(5000.0f);
		filter_.SetRes(1.f); //2.f
    }

    float SyntheticBassDrumClick::Process(float in)
    {
        //SLOPE(lp_, in, 0.5f, 0.1f);
		float error = in - lp_;
		lp_ += (error > 0 ? .5f : .1f) * error;
		
        fonepole(hp_, lp_, 0.04f);
        filter_.Process(lp_ - hp_);
		return filter_.Low();
	}

void SyntheticBassDrumAttackNoise::Init()
    {
        lp_ = 0.0f;
        hp_ = 0.0f;
    }

    float SyntheticBassDrumAttackNoise::Render()
    {
        float sample = stmlib::Random::GetFloat();
        fonepole(lp_, sample, 0.05f);
        fonepole(hp_, lp_, 0.005f);
        return lp_ - hp_;
    }

 void SyntheticBassDrum::Init()
    {
        phase_                = 0.0f;
        phase_noise_          = 0.0f;
        f0_                   = 0.0f;
        fm_                   = 0.0f;
        fm_lp_                = 0.0f;
        body_env_lp_          = 0.0f;
        body_env_             = 0.0f;
        body_env_pulse_width_ = 0;
        fm_pulse_width_       = 0;
        tone_lp_              = 0.0f;
        sustain_gain_         = 0.0f;

        click_.Init();
        noise_.Init();
    }

inline float SyntheticBassDrum::DistortedSine(float phase, float phase_noise, float dirtiness)
    {
        phase += phase_noise * dirtiness;
        
		//MAKE_INTEGRAL_FRACTIONAL(phase);
		int32_t phase_integral = static_cast<int32_t>(phase);
        float phase_fractional = phase - static_cast<float>(phase_integral);
		
        phase          = phase_fractional;
        float triangle = (phase < 0.5f ? phase : 1.0f - phase) * 4.0f - 1.0f;
        float sine     = 2.0f * triangle / (1.0f + fabsf(triangle));
        float clean_sine
            = stmlib::InterpolateWrap(lut_sine, phase + 0.75f, 1024.0f);
        return sine + (1.0f - dirtiness) * (clean_sine - sine);
    }

    inline float SyntheticBassDrum::TransistorVCA(float s, float gain)
    {
        s = (s - 0.6f) * gain;
        return 3.0f * s / (2.0f + fabsf(s)) + gain * 0.3f;
    }

    void SyntheticBassDrum::Render(bool   sustain,
                bool   trigger,
                float  accent,
                float  f0,
                float  tone,
                float  decay,
                float  dirtiness,
                float  fm_envelope_amount,
                float  fm_envelope_decay,
                float* out,
                size_t size)
    {
        decay *= decay;
        fm_envelope_decay *= fm_envelope_decay;

        stmlib::ParameterInterpolator f0_mod(&f0_, f0, size);

        dirtiness *= std::max(1.0f - 8.0f * f0, 0.0f);

        const float fm_decay
            = 1.0f
              - 1.0f
                    / (0.008f * (1.0f + fm_envelope_decay * 4.0f)
                       * kSampleRate);

        const float body_env_decay
            = 1.0f
              - 1.0f / (0.02f * kSampleRate)
                    * powf(2.f, (-decay * 60.0f) / 12.f);
        const float transient_env_decay = 1.0f - 1.0f / (0.005f * kSampleRate);
        const float tone_f              = fmin(
            4.0f * f0 * powf(2.f, (tone * 108.0f) / 12.f), 1.0f);
        const float transient_level = tone;

        if(trigger)
        {
            fm_       = 1.0f;
            body_env_ = transient_env_ = 0.3f + 0.7f * accent;
            body_env_pulse_width_      = kSampleRate * 0.001f;
            fm_pulse_width_            = kSampleRate * 0.0013f;
        }

        stmlib::ParameterInterpolator sustain_gain(
            &sustain_gain_, accent * decay, size);

        while(size--)
        {
            fonepole(phase_noise_, stmlib::Random::GetFloat() - 0.5f, 0.002f);

            float mix = 0.0f;

            if(sustain)
            {
                phase_ += f0_mod.Next();
                if(phase_ >= 1.0f)
                {
                    phase_ -= 1.0f;
                }
                float body = DistortedSine(phase_, phase_noise_, dirtiness);
                mix -= TransistorVCA(body, sustain_gain.Next());
            }
            else
            {
                if(fm_pulse_width_)
                {
                    --fm_pulse_width_;
                    phase_ = 0.25f;
                }
                else
                {
                    fm_ *= fm_decay;
                    float fm = 1.0f + fm_envelope_amount * 3.5f * fm_lp_;
                    phase_ += fmin(f0_mod.Next() * fm, 0.5f);
                    if(phase_ >= 1.0f)
                    {
                        phase_ -= 1.0f;
                    }
                }

                if(body_env_pulse_width_)
                {
                    --body_env_pulse_width_;
                }
                else
                {
                    body_env_ *= body_env_decay;
                    transient_env_ *= transient_env_decay;
                }

                const float envelope_lp_f = 0.1f;
                fonepole(body_env_lp_, body_env_, envelope_lp_f);
                fonepole(transient_env_lp_, transient_env_, envelope_lp_f);
                fonepole(fm_lp_, fm_, envelope_lp_f);

                float body = DistortedSine(phase_, phase_noise_, dirtiness);
                float transient
                    = click_.Process(body_env_pulse_width_ ? 0.0f : 1.0f)
                      + noise_.Render();

                mix -= TransistorVCA(body, body_env_lp_);
                mix -= transient * transient_env_lp_ * transient_level;
            }

            fonepole(tone_lp_, mix, tone_f);
            *out++ = tone_lp_;
        }
    }