#include <cmath>
#include "dsp.h"
#include "KarplusString.h"
#include <stdlib.h>

using namespace daisysp;

void String::Init(float sample_rate)
{
    sample_rate_ = sample_rate;

    SetFreq(440.f);
    non_linearity_amount_ = .5f;
    brightness_           = .5f;
    damping_              = .5f;

    string_.Init();
    stretch_.Init();
    Reset();

    SetFreq(440.f);
    SetDamping(.8f);
    SetNonLinearity(.1f);
    SetBrightness(.5f);

    crossfade_.Init();
}

void String::Reset()
{
    string_.Reset();
    stretch_.Reset();
    iir_damping_filter_.Init();

    dc_blocker_.Init(sample_rate_);

    dispersion_noise_ = 0.0f;
    curved_bridge_    = 0.0f;
    out_sample_[0] = out_sample_[1] = 0.0f;
    src_phase_                      = 0.0f;
}

float String::Process(const float in)
{
    if(non_linearity_amount_ <= 0.0f)
    {
        non_linearity_amount_ *= -1;
        float ret = ProcessInternal<NON_LINEARITY_CURVED_BRIDGE>(in);
        non_linearity_amount_ *= -1;
        return ret;
    }
    else
    {
        return ProcessInternal<NON_LINEARITY_DISPERSION>(in);
    }
}

void String::SetFreq(float freq)
{
    freq /= sample_rate_;
    frequency_ = fclamp(freq, 0.f, .25f);
}

void String::SetNonLinearity(float non_linearity_amount)
{
    non_linearity_amount_ = fclamp(non_linearity_amount, 0.f, 1.f);
}

void String::SetBrightness(float brightness)
{
    brightness_ = fclamp(brightness, 0.f, 1.f);
}

void String::SetDamping(float damping)
{
    damping_ = fclamp(damping, 0.f, 1.f);
}

template <String::StringNonLinearity non_linearity>
float String::ProcessInternal(const float in)
{
    float brightness = brightness_;

    float delay = 1.0f / frequency_;
    delay       = fclamp(delay, 4.f, kDelayLineSize - 4.0f);

    // If there is not enough delay time in the delay line, we play at the
    // lowest possible note and we upsample on the fly with a shitty linear
    // interpolator. We don't care because it's a corner case (frequency_ < 11.7Hz)
    float src_ratio = delay * frequency_;
    if(src_ratio >= 0.9999f)
    {
        // When we are above 11.7 Hz, we make sure that the linear interpolator
        // does not get in the way.
        src_phase_ = 1.0f;
        src_ratio  = 1.0f;
    }

    float damping_cutoff
        = fmin(12.0f + damping_ * damping_ * 60.0f + brightness * 24.0f, 84.0f);
    float damping_f
        = fmin(frequency_ * powf(2.f, damping_cutoff * kOneTwelfth), 0.499f);

    // Crossfade to infinite decay.
    if(damping_ >= 0.95f)
    {
        float to_infinite = 20.0f * (damping_ - 0.95f);
        brightness += to_infinite * (1.0f - brightness);
        damping_f += to_infinite * (0.4999f - damping_f);
        damping_cutoff += to_infinite * (128.0f - damping_cutoff);
    }

    float temp_f = damping_f;
    iir_damping_filter_.SetFrequency(temp_f);

    float ratio                = powf(2.f, damping_cutoff * kOneTwelfth);
    float damping_compensation = 1.f - 2.f * atanf(1.f / ratio) / (TWOPI_F);

    float stretch_point
        = non_linearity_amount_ * (2.0f - non_linearity_amount_) * 0.225f;
    float stretch_correction = (160.0f / sample_rate_) * delay;
    stretch_correction       = fclamp(stretch_correction, 1.f, 2.1f);

    float noise_amount_sqrt = non_linearity_amount_ > 0.75f
                                  ? 4.0f * (non_linearity_amount_ - 0.75f)
                                  : 0.0f;
    float noise_amount = noise_amount_sqrt * noise_amount_sqrt * 0.1f;
    float noise_filter = 0.06f + 0.94f * brightness * brightness;

    float bridge_curving_sqrt = non_linearity_amount_;
    float bridge_curving = bridge_curving_sqrt * bridge_curving_sqrt * 0.01f;

    float ap_gain = -0.618f * non_linearity_amount_
                    / (0.15f + fabsf(non_linearity_amount_));

    src_phase_ += src_ratio;
    if(src_phase_ > 1.0f)
    {
        src_phase_ -= 1.0f;

        delay   = delay * damping_compensation;
        float s = 0.0f;

        if(non_linearity == NON_LINEARITY_DISPERSION)
        {
            float noise = rand() * kRandFrac - 0.5f;
            fonepole(dispersion_noise_, noise, noise_filter);
            delay *= 1.0f + dispersion_noise_ * noise_amount;
        }
        else
        {
            delay *= 1.0f - curved_bridge_ * bridge_curving;
        }

        if(non_linearity == NON_LINEARITY_DISPERSION)
        {
            float ap_delay   = delay * stretch_point;
            float main_delay = delay
                               - ap_delay * (0.408f - stretch_point * 0.308f)
                                     * stretch_correction;
            if(ap_delay >= 4.0f && main_delay >= 4.0f)
            {
                s = string_.Read(main_delay);
                s = stretch_.Allpass(s, ap_delay, ap_gain);
            }
            else
            {
                s = string_.ReadHermite(delay);
            }
        }
        else
        {
            s = string_.ReadHermite(delay);
        }

        if(non_linearity == NON_LINEARITY_CURVED_BRIDGE)
        {
            float value    = fabsf(s) - 0.025f;
            float sign     = s > 0.0f ? 1.0f : -1.5f;
            curved_bridge_ = (fabsf(value) + value) * sign;
        }

        s += in;
        s = fclamp(s, -20.f, +20.f);

        s = dc_blocker_.Process(s);

        s = iir_damping_filter_.Process(s);
        string_.Write(s);

        out_sample_[1] = out_sample_[0];
        out_sample_[0] = s;
    }

    crossfade_.SetPos(src_phase_);
    return crossfade_.Process(out_sample_[1], out_sample_[0]);
}
