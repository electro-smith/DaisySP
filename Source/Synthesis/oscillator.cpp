#include "dsp.h"
#include "oscillator.h"

using namespace daisysp;
static inline float Polyblep(float phase_inc, float t);

constexpr float TWO_PI_RECIP = 1.0f / TWOPI_F;

float Oscillator::Process()
{
    float out, t;
    switch(waveform_)
    {
        case WAVE_SIN: out = sinf(phase_); break;
        case WAVE_TRI:
            t   = -1.0f + (2.0f * phase_ * TWO_PI_RECIP);
            out = 2.0f * (fabsf(t) - 0.5f);
            break;
        case WAVE_SAW:
            out = -1.0f * (((phase_ * TWO_PI_RECIP * 2.0f)) - 1.0f);
            break;
        case WAVE_RAMP: out = ((phase_ * TWO_PI_RECIP * 2.0f)) - 1.0f; break;
        case WAVE_SQUARE: out = phase_ < PI_F ? (1.0f) : -1.0f; break;
        case WAVE_POLYBLEP_TRI:
            t   = phase_ * TWO_PI_RECIP;
            out = phase_ < PI_F ? 1.0f : -1.0f;
            out += Polyblep(phase_inc_, t);
            out -= Polyblep(phase_inc_, fmodf(t + 0.5f, 1.0f));
            // Leaky Integrator:
            // y[n] = A + x[n] + (1 - A) * y[n-1]
            out       = phase_inc_ * out + (1.0f - phase_inc_) * last_out_;
            last_out_ = out;
            break;
        case WAVE_POLYBLEP_SAW:
            t   = phase_ * TWO_PI_RECIP;
            out = (2.0f * t) - 1.0f;
            out -= Polyblep(phase_inc_, t);
            out *= -1.0f;
            break;
        case WAVE_POLYBLEP_SQUARE:
            t   = phase_ * TWO_PI_RECIP;
            out = phase_ < PI_F ? 1.0f : -1.0f;
            out += Polyblep(phase_inc_, t);
            out -= Polyblep(phase_inc_, fmodf(t + 0.5f, 1.0f));
            out *= 0.707f; // ?
            break;
        default: out = 0.0f; break;
    }
    phase_ += phase_inc_;
    if(phase_ > TWOPI_F)
    {
        phase_ -= TWOPI_F;
        eoc_ = true;
    }
    else
    {
        eoc_ = false;
    }
    eor_ = (phase_ - phase_inc_ < PI_F && phase_ >= PI_F);

    return out * amp_;
}

float Oscillator::CalcPhaseInc(float f)
{
    return (TWOPI_F * f) * sr_recip_;
}

static float Polyblep(float phase_inc, float t)
{
    float dt = phase_inc * TWO_PI_RECIP;
    if(t < dt)
    {
        t /= dt;
        return t + t - t * t - 1.0f;
    }
    else if(t > 1.0f - dt)
    {
        t = (t - 1.0f) / dt;
        return t * t + t + t + 1.0f;
    }
    else
    {
        return 0.0f;
    }
}
