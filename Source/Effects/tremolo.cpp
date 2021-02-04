#include "tremolo.h"
#include <math.h>

using namespace daisysp;

void Tremolo::Init(float sample_rate)
{
    sample_rate_ = sample_rate;

    osc_.Init(sample_rate_);
    SetDepth(1.f);
    SetFreq(1.f);
}

float Tremolo::Process(float in)
{
    float modsig = dc_os_ + osc_.Process();
    return in * modsig;
}

void Tremolo::SetFreq(float freq)
{
    osc_.SetFreq(freq);
}

void Tremolo::SetWaveform(int waveform)
{
    osc_.SetWaveform(waveform);
}
void Tremolo::SetDepth(float depth)
{
    depth = fclamp(depth, 0.f, 1.f);
    depth *= .5f;
    osc_.SetAmp(depth);
    dc_os_ = 1.f - depth;
}