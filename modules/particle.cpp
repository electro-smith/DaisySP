#include "dsp.h"
#include "particle.h"
#include <math.h>

using namespace daisysp;

void Particle::Init(float sample_rate)
{
    sample_rate_ = sample_rate;
	
    sync_        = false;
	aux_ = 0.f;
	SetFreq(500.f);
	resonance_ = .5f;
	density_ = .5f;
	gain_ = 1.f;
	spread_ = .5f;

	SetRandomFreq(sample_rate_ / 48.f); //48 is the default block size
    rand_phase_ = 0.f;

    pre_gain_ = 0.0f;
    filter_.Init(sample_rate_);
}

float Particle::Process()
{
    float u = sync_ ? density_ : GetFloat();
    sync_   = false;

    float s = 0.0f;

    rand_phase_ += rand_freq_;

    if(u <= density_)
    {
        s = u * gain_;
        if(rand_phase_ >= 1.f)
        {
            rand_phase_ -= 1.f;

            const float u = 2.0f * GetFloat() - 1.0f;
            const float f
                = fmin(powf(2.f, ratiofrac_ * spread_ * u) * frequency_, 0.25f);
            pre_gain_
                = 0.5f / fastroot(resonance_ * f * fastroot(density_, 2), 2);
            filter_.SetFreq(f);
            filter_.SetRes(resonance_);
        }
    }
    aux_ = s;

    filter_.Process(pre_gain_ * s);
    return filter_.Band();
}

float Particle::ProcessAux()
{
    return aux_;
}

void Particle::SetFreq(float freq)
{
    freq /= sample_rate_;
    frequency_ = fclamp(freq, 0.f, 1.f);
}

void Particle::SetResonance(float resonance)
{
    resonance_ = resonance;
}

void Particle::SetRandomFreq(float freq)
{
    freq /= sample_rate_;
    rand_freq_ = fclamp(freq, 0.f, 1.f);
}

void Particle::SetDensity(float density)
{
    density_ = density;
}

void Particle::SetGain(float gain)
{
    gain_ = fclamp(gain, 0.f, 1.f);
}

void Particle::SetSpread(float spread)
{
    spread_ = spread;
}

void Particle::Sync()
{
    sync_ = true;
}

inline float Particle::GetFloat()
{
    rng_state_ = rng_state_ * 1664525L + 1013904223L;
    return static_cast<float>(rng_state_) / 4294967296.0f;
}