#include "dsp.h"
#include "hihat.h"
#include <math.h>

using namespace daisysp;

void SquareNoise::Init()
{
    for(int i = 0; i < 6; i++)
    {
        phase_[i] = 0;
    }
}

void SquareNoise::Process(float  f0,
                          float* temp_1,
                          float* temp_2,
                          float* out,
                          size_t size)
{
    const float ratios[6] = {// Nominal f0: 414 Hz
                             1.0f,
                             1.304f,
                             1.466f,
                             1.787f,
                             1.932f,
                             2.536f};

    uint32_t increment[6];
    uint32_t phase[6];
    for(int i = 0; i < 6; ++i)
    {
        float f = f0 * ratios[i];
        if(f >= 0.499f)
            f = 0.499f;
        increment[i] = static_cast<uint32_t>(f * 4294967296.0f);
        phase[i]     = phase_[i];
    }

    while(size--)
    {
        phase[0] += increment[0];
        phase[1] += increment[1];
        phase[2] += increment[2];
        phase[3] += increment[3];
        phase[4] += increment[4];
        phase[5] += increment[5];
        uint32_t noise = 0;
        noise += (phase[0] >> 31);
        noise += (phase[1] >> 31);
        noise += (phase[2] >> 31);
        noise += (phase[3] >> 31);
        noise += (phase[4] >> 31);
        noise += (phase[5] >> 31);
        *out++ = 0.33f * static_cast<float>(noise) - 1.0f;
    }

    for(int i = 0; i < 6; ++i)
    {
        phase_[i] = phase[i];
    }
}

void RingModNoise::Init(float sample_rate)
{
    sample_rate_ = sample_rate;

    for(int i = 0; i < 6; ++i)
    {
        oscillator_[i].Init(sample_rate_);
    }
}

void RingModNoise::Process(float  f0,
                           float* temp_1,
                           float* temp_2,
                           float* out,
                           size_t size)
{
    const float ratio = f0 / (0.01f + f0);
    const float f1a   = 200.0f / sample_rate_ * ratio;
    const float f1b   = 7530.0f / sample_rate_ * ratio;
    const float f2a   = 510.0f / sample_rate_ * ratio;
    const float f2b   = 8075.0f / sample_rate_ * ratio;
    const float f3a   = 730.0f / sample_rate_ * ratio;
    const float f3b   = 10500.0f / sample_rate_ * ratio;

    for(size_t i = 0; i < size; i++)
    {
        out[i] = 0.f;
    }

    ProcessPair(&oscillator_[0], f1a, f1b, temp_1, temp_2, out, size);
    ProcessPair(&oscillator_[2], f2a, f2b, temp_1, temp_2, out, size);
    ProcessPair(&oscillator_[4], f3a, f3b, temp_1, temp_2, out, size);
}

void RingModNoise::ProcessPair(Oscillator* osc,
                               float       f1,
                               float       f2,
                               float*      temp_1,
                               float*      temp_2,
                               float*      out,
                               size_t      size)
{
    osc[0].SetWaveform(Oscillator::WAVE_SQUARE);
    osc[0].SetFreq(f1 * sample_rate_);
    *temp_1 = osc[0].Process();

    osc[1].SetWaveform(Oscillator::WAVE_SAW);
    osc[1].SetFreq(f2 * sample_rate_);
    *temp_2 = osc[1].Process();
    while(size--)
    {
        *out++ += *temp_1++ * *temp_2++;
    }
}