#include "modules/modalvoice.h"
#include <algorithm>

using namespace daisysp;

void ModalVoice::Init(float sample_rate)
{
    sample_rate_ = sample_rate;

    excitation_filter_.Init();
    resonator_.Init(0.015f, 24, sample_rate_);
    excitation_filter_.Init();
    dust_.Init();
}

void ModalVoice::Process(bool   sustain,
                         bool   trigger,
                         float  accent,
                         float  f0,
                         float  structure,
                         float  brightness,
                         float  damping,
                         float* temp,
                         float* out,
                         float* aux,
                         size_t size)
{
    const float density = brightness * brightness;

    brightness += 0.25f * accent * (1.0f - brightness);
    damping += 0.25f * accent * (1.0f - damping);

    const float range  = sustain ? 36.0f : 60.0f;
    const float f      = sustain ? 4.0f * f0 : 2.0f * f0;
    const float cutoff = fmin(
        f
            * powf(2.f,
                   kOneTwelfth
                       * ((brightness * (2.0f - brightness) - 0.5f) * range)),
        0.499f);
    const float q = sustain ? 0.7f : 1.5f;

    // Synthesize excitation signal.
    if(sustain)
    {
        const float dust_f = 0.00005f + 0.99995f * density * density;
        dust_.SetDensity(dust_f);
        for(size_t i = 0; i < size; ++i)
        {
            temp[i] = dust_.Process() * (4.0f - dust_f * 3.0f) * accent;
        }
    }
    else
    {
        for(size_t i = 0; i < size; i++)
        {
            temp[i] = 0.f;
        }

        if(trigger)
        {
            const float attenuation = 1.0f - damping * 0.5f;
            const float amplitude   = (0.12f + 0.08f * accent) * attenuation;
            temp[0]                 = amplitude
                      * powf(2.f, kOneTwelfth * (cutoff * cutoff * 24.0f))
                      / cutoff;
        }
    }
    const float one = 1.0f;
    excitation_filter_.Process<ResonatorSvf<1>::LOW_PASS, false>(
        &cutoff, &q, &one, temp[0], temp);
    for(size_t i = 0; i < size; ++i)
    {
        aux[i] += temp[i];
    }

    resonator_.SetFreq(f0);
    resonator_.SetStructure(structure);
    resonator_.SetBrightness(brightness);
    resonator_.SetDamping(damping);

    *out = resonator_.Process(temp[0]);
}