#include "dsp.h"
#include "chorus.h"
#include <math.h>

using namespace daisysp;

//ChorusEngine stuff
void ChorusEngine::Init(float sample_rate)
{
    sample_rate_ = sample_rate;

    del_.Init();
    lfo_amp_  = 0.f;
    feedback_ = .2f;
    SetDelay(.75);

    lfo_phase_ = 0.f;
    SetLfoFreq(.3f);
    SetLfoDepth(.9f);
}

float ChorusEngine::Process(float in)
{
    float lfo_sig = ProcessLfo();
    del_.SetDelay(lfo_sig + delay_);

    float out = del_.Read();
    del_.Write(in + out * feedback_);

    return (in + out) * .5f; //equal mix
}

void ChorusEngine::SetLfoDepth(float depth)
{
    depth    = fclamp(depth, 0.f, .93f);
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
    ms     = fmax(.1f, ms);
    delay_ = ms * .001f * sample_rate_; //ms to samples

    lfo_amp_ = fmin(lfo_amp_, delay_); //clip this if needed
}

void ChorusEngine::SetFeedback(float feedback)
{
    feedback_ = fclamp(feedback, 0.f, 1.f);
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
    SetPan(.25f, .75f);

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

void Chorus::SetPan(float panl, float panr)
{
    pan_[0] = fclamp(panl, 0.f, 1.f);
    pan_[1] = fclamp(panr, 0.f, 1.f);
}

void Chorus::SetPan(float pan)
{
    SetPan(pan, pan);
}

void Chorus::SetLfoDepth(float depthl, float depthr)
{
    engines_[0].SetLfoDepth(depthl);
    engines_[1].SetLfoDepth(depthr);
}

void Chorus::SetLfoDepth(float depth)
{
    SetLfoDepth(depth, depth);
}

void Chorus::SetLfoFreq(float freql, float freqr)
{
    engines_[0].SetLfoFreq(freql);
    engines_[1].SetLfoFreq(freqr);
}

void Chorus::SetLfoFreq(float freq)
{
    SetLfoFreq(freq, freq);
}

void Chorus::SetDelay(float delayl, float delayr)
{
    engines_[0].SetDelay(delayl);
    engines_[1].SetDelay(delayr);
}

void Chorus::SetDelay(float delay)
{
    SetDelay(delay, delay);
}

void Chorus::SetDelayMs(float msl, float msr)
{
    engines_[0].SetDelayMs(msl);
    engines_[1].SetDelayMs(msr);
}

void Chorus::SetDelayMs(float ms)
{
    SetDelayMs(ms, ms);
}

void Chorus::SetFeedback(float feedbackl, float feedbackr)
{
    engines_[0].SetFeedback(feedbackl);
    engines_[1].SetFeedback(feedbackr);
}

void Chorus::SetFeedback(float feedback)
{
    SetFeedback(feedback, feedback);
}
