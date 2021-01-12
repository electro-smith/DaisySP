#include <cmath>
#include "modules/dsp.h"
#include "modules/string.h"

using namespace daisysp;

void String::Init(float sample_rate)
{
    sample_rate_ = sample_rate;

    string_.Init();
    stretch_.Init();
    delay_ = 100.0f;
    Reset();

	crossfade_.Init();
}

void String::Reset()
{
    string_.Reset();
    stretch_.Reset();
    iir_damping_filter_.Init(sample_rate_);
    dc_blocker_.Init(1.0f - 20.0f / sample_rate_);
    dispersion_noise_ = 0.0f;
    curved_bridge_    = 0.0f;
    out_sample_[0] = out_sample_[1] = 0.0f;
    src_phase_                      = 0.0f;
}

float String::Process(float        f0,
                     float        non_linearity_amount,
                     float        brightness,
                     float        damping,
                     const float* in,
                     float*       out,
                     size_t       size)
{
    if(non_linearity_amount <= 0.0f)
    {
        return ProcessInternal<STRING_NON_LINEARITY_CURVED_BRIDGE>(
            f0, -non_linearity_amount, brightness, damping, in, out, size);
    }
    else
    {
        return ProcessInternal<STRING_NON_LINEARITY_DISPERSION>(
            f0, non_linearity_amount, brightness, damping, in, out, size);
    }
}

template <StringNonLinearity non_linearity>
float  String::ProcessInternal(float        f0,
                             float        non_linearity_amount,
                             float        brightness,
                             float        damping,
                             const float* in,
                             float*       out,
                             size_t       size)
{
    float delay = 1.0f / f0;
    delay       = fclamp(delay, 4.f, kDelayLineSize - 4.0f);

    // If there is not enough delay time in the delay line, we play at the
    // lowest possible note and we upsample on the fly with a shitty linear
    // interpolator. We don't care because it's a corner case (f0 < 11.7Hz)
    float src_ratio = delay * f0;
    if(src_ratio >= 0.9999f)
    {
        // When we are above 11.7 Hz, we make sure that the linear interpolator
        // does not get in the way.
        src_phase_ = 1.0f;
        src_ratio  = 1.0f;
    }

    float damping_cutoff
        = fmin(12.0f + damping * damping * 60.0f + brightness * 24.0f, 84.0f);
    float damping_f
        = fmin(f0 * powf(2.f, damping_cutoff * ratio_frac_), 0.499f);

    // Crossfade to infinite decay.
    if(damping >= 0.95f)
    {
        float to_infinite = 20.0f * (damping - 0.95f);
        brightness += to_infinite * (1.0f - brightness);
        damping_f += to_infinite * (0.4999f - damping_f);
        damping_cutoff += to_infinite * (128.0f - damping_cutoff);
    }

    iir_damping_filter_.SetFreq(damping_f);
    iir_damping_filter_.SetRes(0.5f);


    float damping_compensation
        = Interpolate(lut_svf_shift, damping_cutoff, 1.0f);

    // Linearly interpolate delay time.
    //ParameterInterpolator delay_modulation(&delay_, delay * damping_compensation, size);

    float stretch_point
        = non_linearity_amount * (2.0f - non_linearity_amount) * 0.225f;
    float stretch_correction = (160.0f / sample_rate_) * delay;
    stretch_correction       = fclamp(stretch_correction, 1.f, 2.1f);

    float noise_amount_sqrt = non_linearity_amount > 0.75f
                                  ? 4.0f * (non_linearity_amount - 0.75f)
                                  : 0.0f;
    float noise_amount = noise_amount_sqrt * noise_amount_sqrt * 0.1f;
    float noise_filter = 0.06f + 0.94f * brightness * brightness;

    float bridge_curving_sqrt = non_linearity_amount;
    float bridge_curving = bridge_curving_sqrt * bridge_curving_sqrt * 0.01f;

    float ap_gain = -0.618f * non_linearity_amount
                    / (0.15f + fabsf(non_linearity_amount));

	//while
        src_phase_ += src_ratio;
        if(src_phase_ > 1.0f)
        {
            src_phase_ -= 1.0f;

            float delay = delay * damping_compensation;
            float s     = 0.0f;

            if(non_linearity == STRING_NON_LINEARITY_DISPERSION)
            {
                float noise = random() * rand_frac_ - 0.5f;
                fonepole(dispersion_noise_, noise, noise_filter);
                delay *= 1.0f + dispersion_noise_ * noise_amount;
            }
            else
            {
                delay *= 1.0f - curved_bridge_ * bridge_curving;
            }

            if(non_linearity == STRING_NON_LINEARITY_DISPERSION)
            {
                float ap_delay   = delay * stretch_point;
                float main_delay = delay
                                   - ap_delay
                                         * (0.408f - stretch_point * 0.308f)
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

            if(non_linearity == STRING_NON_LINEARITY_CURVED_BRIDGE)
            {
                float value    = fabsf(s) - 0.025f;
                float sign     = s > 0.0f ? 1.0f : -1.5f;
                curved_bridge_ = (fabsf(value) + value) * sign;
            }

            s += *in;
            s = fclamp(s, -20.f, +20.f);

            dc_blocker_.Process(s);
			iir_damping_filter_.Process(s);
            s = iir_damping_filter_.Low();
            string_.Write(s);

            out_sample_[1] = out_sample_[0];
            out_sample_[0] = s;
        }
    
	crossfade_.SetPos(src_phase_);
	return crossfade_.Process(out_sample_[1], out_sample_[0]);
}