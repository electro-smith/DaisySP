#include "dsp.h"
#include "analogbassdrum.h"
#include <math.h>

using namespace daisysp;

void AnalogBassDrum::Init(float sample_rate)
{
    sample_rate_ = sample_rate;

    pulse_remaining_samples_    = 0;
    fm_pulse_remaining_samples_ = 0;
    pulse_                      = 0.0f;
    pulse_height_               = 0.0f;
    pulse_lp_                   = 0.0f;
    fm_pulse_lp_                = 0.0f;
    retrig_pulse_               = 0.0f;
    lp_out_                     = 0.0f;
    tone_lp_                    = 0.0f;
    sustain_gain_               = 0.0f;
	phase_ = 0.f;

    resonator_.Init(sample_rate_);
}

inline float AnalogBassDrum::Diode(float x)
{
    if(x >= 0.0f)
    {
        return x;
    }
    else
    {
        x *= 2.0f;
        return 0.7f * x / (1.0f + fabsf(x));
    }
}

float AnalogBassDrum::Process(bool   sustain,
                             bool   trigger,
                             float  accent,
                             float  f0,
                             float  tone,
                             float  decay,
                             float  attack_fm_amount,
                             float  self_fm_amount)
{
    const int   kTriggerPulseDuration = 1.0e-3 * sample_rate_;
    const int   kFMPulseDuration      = 6.0e-3 * sample_rate_;
    const float kPulseDecayTime       = 0.2e-3 * sample_rate_;
    const float kPulseFilterTime      = 0.1e-3 * sample_rate_;
    const float kRetrigPulseDuration  = 0.05f * sample_rate_;

    const float scale = 0.001f / f0;
    const float q     = 1500.0f * powf(2.f, ratio_frac_ * decay * 80.0f);
    const float tone_f
        = fmin(4.0f * f0 * powf(2.f, ratio_frac_ * tone * 108.0f), 1.0f);
    const float exciter_leak = 0.08f * (tone + 0.25f);


    if(trigger)
    {
        pulse_remaining_samples_    = kTriggerPulseDuration;
        fm_pulse_remaining_samples_ = kFMPulseDuration;
        pulse_height_               = 3.0f + 7.0f * accent;
        lp_out_                     = 0.0f;
    }

        // Q39 / Q40
        float pulse = 0.0f;
        if(pulse_remaining_samples_)
        {
            --pulse_remaining_samples_;
            pulse = pulse_remaining_samples_ ? pulse_height_
                                             : pulse_height_ - 1.0f;
            pulse_ = pulse;
        }
        else
        {
            pulse_ *= 1.0f - 1.0f / kPulseDecayTime;
            pulse = pulse_;
        }
        if(sustain)
        {
            pulse = 0.0f;
        }

        // C40 / R163 / R162 / D83
        fonepole(pulse_lp_, pulse, 1.0f / kPulseFilterTime);
        pulse = Diode((pulse - pulse_lp_) + pulse * 0.044f);

        // Q41 / Q42
        float fm_pulse = 0.0f;
        if(fm_pulse_remaining_samples_)
        {
            --fm_pulse_remaining_samples_;
            fm_pulse = 1.0f;
            // C39 / C52
            retrig_pulse_ = fm_pulse_remaining_samples_ ? 0.0f : -0.8f;
        }
        else
        {
            // C39 / R161
            retrig_pulse_ *= 1.0f - 1.0f / kRetrigPulseDuration;
        }
        if(sustain)
        {
            fm_pulse = 0.0f;
        }
        fonepole(fm_pulse_lp_, fm_pulse, 1.0f / kPulseFilterTime);

        // Q43 and R170 leakage
        float punch = 0.7f + Diode(10.0f * lp_out_ - 1.0f);

        // Q43 / R165
        float attack_fm = fm_pulse_lp_ * 1.7f * attack_fm_amount;
        float self_fm   = punch * 0.08f * self_fm_amount;
        float f         = f0 * (1.0f + attack_fm + self_fm);
        f               = fclamp(f, 0.0f, 0.4f);

        float resonator_out;
        if(sustain)
        {
            sustain_gain_ = accent * decay;
			phase_ += f;
			phase_ = phase_ >= 1.f ? phase_ - 1.f : phase_;

			resonator_out = sin(TWOPI_F * phase_) * sustain_gain_;
			lp_out_ = cos(TWOPI_F * phase_) * sustain_gain_;			
        }
        else
        {
            resonator_.SetFreq(f * sample_rate_);
            resonator_.SetRes(1.0f + q * f);

            resonator_.Process((pulse - retrig_pulse_ * 0.2f) * scale);
            resonator_out = resonator_.Band();
            lp_out_       = resonator_.Low();
        }

        fonepole(tone_lp_, pulse * exciter_leak + resonator_out, tone_f);

        return tone_lp_;
    
}
