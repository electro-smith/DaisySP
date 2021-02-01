#include "dsp.h"
#include "chorus.h"
#include <math.h>

using namespace daisysp;

//ChorusEngine stuff
void ChorusEngine::Init(float sample_rate)
{
    sample_rate_ = sample_rate;

    del_.Init();
    lfo_amp_ = 0.f;
    SetDelay(.75);

    lfo_phase_ = 0.f;
    SetLfoFreq(.3);
    SetLfoDepth(.9);
}

float ChorusEngine::Process(float in)
{
    float lfo_sig = ProcessLfo();
    del_.SetDelay(lfo_sig + delay_);

    float out = del_.Read();
    del_.Write(in);

    return (in + out) * .5f; //equal mix
}

void ChorusEngine::SetLfoDepth(float depth)
{
    depth    = fclamp(depth, 0.f, 1.f);
    lfo_amp_ = depth * delay_;
}

void ChorusEngine::SetLfoFreq(float freq)
{
    freq = 4.f * freq / sample_rate_;
    freq *= lfo_freq_ < 0.f ? -1.f : 1.f;  //if we're headed down, keep going
    lfo_freq_ = fclamp(freq, -.25f, .25f); //clip at +/- .125 * sr
}

void ChorusEngine::SetDelay(float delay)
{
    delay = (.1f + delay * 7.9f); //.1 to 8 ms
    SetDelayMs(delay);
}

void ChorusEngine::SetDelayMs(float ms)
{
    ms     = fmax(.1, ms);
    delay_ = ms * .001f * sample_rate_; //ms to samples

    lfo_amp_ = fmin(lfo_amp_, delay_); //clip this if needed
}

float ChorusEngine::ProcessLfo()
{
    lfo_phase_ += lfo_freq_;

    //wrap around and flip direction
    if(lfo_phase_ > 1.f)
    {
        lfo_phase_ = 1.f - (lfo_phase_ - 1.f);
        lfo_freq_ *= -1.f;
    }
    else if(lfo_phase_ < -1.f)
    {
        lfo_phase_ = -1.f - (lfo_phase_ + 1.f);
        lfo_freq_ *= -1.f;
    }

    return lfo_phase_ * lfo_amp_;
}

//Chorus Stuff
void Chorus::Init(float sample_rate)
{
    engines_[0].Init(sample_rate);
    engines_[1].Init(sample_rate);
    SetPanBoth(.25f, .75f);

    gain_frac_ = .5f;
    sigl_ = sigr_ = 0.f;
}

float Chorus::Process(float in)
{
    sigl_ = 0.f;
    sigr_ = 0.f;

    for(int i = 0; i < 2; i++)
    {
        float sig = engines_[i].Process(in);
        sigl_ += (1.f - pan_[i]) * sig;
        sigr_ += pan_[i] * sig;
    }

    sigl_ *= gain_frac_;
    sigr_ *= gain_frac_;

    return sigl_;
}

float Chorus::GetLeft()
{
    return sigl_;
}

float Chorus::GetRight()
{
    return sigr_;
}

void Chorus::SetPan(float pan, int delnum)
{
    pan_[delnum] = fclamp(pan, 0.f, 1.f);
}

void Chorus::SetPanBoth(float panl, float panr)
{
    SetPan(panl, 0);
    SetPan(panr, 1);
}

void Chorus::SetLfoDepth(float depth, int delnum)
{
    engines_[delnum].SetLfoDepth(depth);
}

void Chorus::SetLfoFreq(float freq, int delnum)
{
    engines_[delnum].SetLfoFreq(freq);
}

void Chorus::SetDelay(float ms, int delnum)
{
    engines_[delnum].SetDelay(ms);
}

void Chorus::SetLfoDepthAll(float depth)
{
    engines_[0].SetLfoDepth(depth);
    engines_[1].SetLfoDepth(depth);
}

void Chorus::SetLfoFreqAll(float freq)
{
    engines_[0].SetLfoFreq(freq);
    engines_[1].SetLfoFreq(freq);
}

void Chorus::SetDelayAll(float ms)
{
    engines_[0].SetDelay(ms);
    engines_[1].SetDelay(ms);
}