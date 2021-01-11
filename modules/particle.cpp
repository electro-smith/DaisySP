#include "dsp.h"
#include "particle.h"
#include <math.h>

using namespace daisysp;

inline void Particle::Init(float sample_rate)
{
    sample_rate_ = sample_rate;

    pre_gain_ = 0.0f;
    filter_.Init(sample_rate_);
}

inline float Particle::Process(bool  sync,
                               float density,
                               float gain,
                               float frequency,
                               float spread,
                               float q)
{
    float u = GetFloat();
    if(sync)
    {
        u = density;
    }
    bool can_radomize_frequency = true;
    //    while (size--) {
    float s = 0.0f;
    if(u <= density)
    {
        s = u * gain;
        if(can_radomize_frequency)
        {
            const float u = 2.0f * GetFloat() - 1.0f;
            const float f = fmin(
                    powf(2.f, ratiofrac_ * spread * u) * frequency, 0.25f);
            pre_gain_ = 0.5f / fastroot(q * f * fastroot(density, 2), 2);
            filter_.SetFreq(f);
			filter_.SetRes(q);

            // Keep the cutoff constant for this whole block.
            can_radomize_frequency = false;
        }
    }
    aux_ = s;
    u    = GetFloat();
    //}

    filter_.Process(pre_gain_ * s);
	return filter_.Band();
}

float Particle::ProcessAux()
{
    return aux_;
}

inline float Particle::GetFloat()
{
    rng_state_ = rng_state_ * 1664525L + 1013904223L;
    return static_cast<float>(rng_state_) / 4294967296.0f;
}