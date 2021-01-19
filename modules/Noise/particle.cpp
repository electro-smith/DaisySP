#include "dsp.h"
#include "particle.h"
#include <math.h>

using namespace daisysp;

void Particle::Init(float sample_rate)
{
    sample_rate_ = sample_rate;

    sync_ = false;
    aux_  = 0.f;
    SetFreq(440.f);
    resonance_ = .9f;
    density_   = .5f;
    gain_      = 1.f;
    spread_    = 1.f;

    SetRandomFreq(sample_rate_ / 48.f); //48 is the default block size
    rand_phase_ = 0.f;

    pre_gain_ = 0.0f;
    filter_.Init(sample_rate_);
    filter_.SetDrive(.7f);
}

float Particle::Process()
{
    float u = rand() * kRandFrac;
    float s = 0.0f;

    if(u <= density_ || sync_)
    {
        s = u <= density_ ? u * gain_ : s;
        rand_phase_ += rand_freq_;

        if(rand_phase_ >= 1.f || sync_)
        {
            rand_phase_ = rand_phase_ >= 1.f ? rand_phase_ - 1.f : rand_phase_;

            const float u = 2.0f * rand() * kRandFrac - 1.0f;
            const float f
                = fmin(powf(2.f, kRatioFrac * spread_ * u) * frequency_, .25f);
            pre_gain_ = 0.5f / sqrtf(resonance_ * f * sqrtf(density_));
            filter_.SetFreq(f * sample_rate_);
            filter_.SetRes(resonance_);
        }
    }
    aux_ = s;

    filter_.Process(pre_gain_ * s);
    return filter_.Band();
}

float Particle::GetNoise()
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
    resonance_ = fclamp(resonance, 0.f, 1.f);
}

void Particle::SetRandomFreq(float freq)
{
    freq /= sample_rate_;
    rand_freq_ = fclamp(freq, 0.f, 1.f);
}

void Particle::SetDensity(float density)
{
    density_ = fclamp(density * .3f, 0.f, 1.f);
}

void Particle::SetGain(float gain)
{
    gain_ = fclamp(gain, 0.f, 1.f);
}

void Particle::SetSpread(float spread)
{
    spread_ = spread < 0.f ? 0.f : spread;
}

void Particle::SetSync(bool sync)
{
    sync_ = sync;
}