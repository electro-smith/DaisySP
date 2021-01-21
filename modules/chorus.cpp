#include "modules/dsp.h"
#include "modules/chorus.h"
#include <math.h>

using namespace daisysp;

void Chorus::Init(float sample_rate)
{
    sample_rate_ = sample_rate;

    pre_gain_ = 1.f / (NUM_DEL + 1);

    for(int i = 0; i < NUM_DEL; i++)
    {
        del_[i].Init();
        lfo_amp_[i] = 0.f;
        SetDelay(2 + 2 * i, i);

        lfo_phase_[i] = 0.f;
        SetLfoFreq(.5 + .05 * i, i);
        SetLfoDepth(2, i);
    }
}

float Chorus::ProcessLfo(int idx)
{
    lfo_phase_[idx] += lfo_freq_[idx];

    //wrap around and flip direction
    if(lfo_phase_[idx] > 1.f)
    {
        lfo_phase_[idx] = 1.f - (lfo_phase_[idx] - 1.f);
        lfo_freq_[idx] *= -1.f;
    }
    else if(lfo_phase_[idx] < -1.f)
    {
        lfo_phase_[idx] = -1.f - (lfo_phase_[idx] + 1.f);
        lfo_freq_[idx] *= -1.f;
    }

    return lfo_phase_[idx] * lfo_amp_[idx];
}

float Chorus::Process(float in)
{
    float ret = 0.f;

    for(int i = 0; i < NUM_DEL; i++)
    {
        float lfo_sig = ProcessLfo(i);
        del_[i].SetDelay(lfo_sig + delay_[i]);

        ret += del_[i].Read();
        del_[i].Write(in);
    }

    return in * pre_gain_ + ret * pre_gain_;
}

void Chorus::SetLfoDepth(float depth, int idx)
{
    if(idx < 0 || idx >= NUM_DEL)
    {
        return;
    }

    depth *= sample_rate_ * .001f;
    lfo_amp_[idx] = fclamp(depth, 0.f, delay_[idx]);
}

void Chorus::SetLfoFreq(float freq, int idx)
{
    if(idx < 0 || idx >= NUM_DEL)
    {
        return;
    }

    freq = 4.f * freq / sample_rate_;
    freq
        *= lfo_freq_[idx] < 0.f ? -1.f : 1.f; //if we're headed down, keep going
    lfo_freq_[idx] = fclamp(freq, -.25f, .25f); //clip at +/- .125 * sr
}

void Chorus::SetDelay(float ms, int idx)
{
    if(idx < 0 || idx >= NUM_DEL)
    {
        return;
    }

    ms *= .001f * sample_rate_;
    delay_[idx] = fmax(ms, 0.0001f * sample_rate_);

    lfo_amp_[idx] = fmin(lfo_amp_[idx], delay_[idx]); //clip this if needed
}

void Chorus::SetLfoDepthAll(float depth)
{
    for(int i = 0; i < NUM_DEL; i++)
    {
        SetLfoDepth(depth, i);
    }
}

void Chorus::SetLfoFreqAll(float freq)
{
    for(int i = 0; i < NUM_DEL; i++)
    {
        SetLfoFreq(freq, i);
    }
}

void Chorus::SetDelayAll(float ms)
{
    for(int i = 0; i < NUM_DEL; i++)
    {
        SetDelay(ms, i);
    }
}