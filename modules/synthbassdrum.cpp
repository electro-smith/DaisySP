#include "dsp.h"
#include "synthbassdrum.h"
#include <math.h>

using namespace daisysp;

void Init()
{
    lp_ = 0.0f;
    hp_ = 0.0f;
    filter_.Init();
    filter_.set_f_q<stmlib::FREQUENCY_FAST>(5000.0f / kSampleRate, 2.0f);
}

float Process(float in)
{
    SLOPE(lp_, in, 0.5f, 0.1f);
    ONE_POLE(hp_, lp_, 0.04f);
    return filter_.Process<stmlib::FILTER_MODE_LOW_PASS>(lp_ - hp_);
}

void Init()
{
    lp_ = 0.0f;
    hp_ = 0.0f;
}

float Render()
{
    float sample = stmlib::Random::GetFloat();
    ONE_POLE(lp_, sample, 0.05f);
    ONE_POLE(hp_, lp_, 0.005f);
    return lp_ - hp_;
}

void Init()
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

inline float DistortedSine(float phase, float phase_noise, float dirtiness)
{
    phase += phase_noise * dirtiness;
    MAKE_INTEGRAL_FRACTIONAL(phase);
    phase          = phase_fractional;
    float triangle = (phase < 0.5f ? phase : 1.0f - phase) * 4.0f - 1.0f;
    float sine     = 2.0f * triangle / (1.0f + fabsf(triangle));
    float clean_sine
        = stmlib::InterpolateWrap(lut_sine, phase + 0.75f, 1024.0f);
    return sine + (1.0f - dirtiness) * (clean_sine - sine);
}

inline float TransistorVCA(float s, float gain)
{
    s = (s - 0.6f) * gain;
    return 3.0f * s / (2.0f + fabsf(s)) + gain * 0.3f;
}

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
            size_t size)
{
    decay *= decay;
    fm_envelope_decay *= fm_envelope_decay;

    stmlib::ParameterInterpolator f0_mod(&f0_, f0, size);

    dirtiness *= std::max(1.0f - 8.0f * f0, 0.0f);

    const float fm_decay
        = 1.0f
          - 1.0f / (0.008f * (1.0f + fm_envelope_decay * 4.0f) * kSampleRate);

    const float body_env_decay
        = 1.0f
          - 1.0f / (0.02f * kSampleRate)
                * stmlib::SemitonesToRatio(-decay * 60.0f);
    const float transient_env_decay = 1.0f - 1.0f / (0.005f * kSampleRate);
    const float tone_f
        = std::min(4.0f * f0 * stmlib::SemitonesToRatio(tone * 108.0f), 1.0f);
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
        ONE_POLE(phase_noise_, stmlib::Random::GetFloat() - 0.5f, 0.002f);

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
                phase_ += std::min(f0_mod.Next() * fm, 0.5f);
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
            ONE_POLE(body_env_lp_, body_env_, envelope_lp_f);
            ONE_POLE(transient_env_lp_, transient_env_, envelope_lp_f);
            ONE_POLE(fm_lp_, fm_, envelope_lp_f);

            float body = DistortedSine(phase_, phase_noise_, dirtiness);
            float transient
                = click_.Process(body_env_pulse_width_ ? 0.0f : 1.0f)
                  + noise_.Render();

            mix -= TransistorVCA(body, body_env_lp_);
            mix -= transient * transient_env_lp_ * transient_level;
        }

        ONE_POLE(tone_lp_, mix, tone_f);
        *out++ = tone_lp_;
    }
}