#include "dsp.h"
#include "phaser.h"
#include <math.h>

// Shensley WIP notes:
// started hacking on a zero-delay implementation:
// This equation fro getting the coeff comes from:
// https://www.kvraudio.com/forum/viewtopic.php?f=33&t=245056&start=15
// and a related thread:
// https://www.kvraudio.com/forum/viewtopic.php?t=300850
//
// Equation corroborated and implementation mostly based on the concepts discussed
// here:
// https://ccrma.stanford.edu/realsimple/DelayVar/Phasing_First_Order_Allpass_Filters.html
//
// rTheta = rFreq * krPi * rInvSmplRate;
// rD = tan(rTheta);
// rG = (1.0 - rD) / (1.0 + rD);

using namespace daisysp;

static float cot(float x)
{
    return cos(x) / sin(x);
}

//PhaserEngine stuff
void PhaserEngine::Init(float sample_rate)
{
    sample_rate_ = sample_rate;

    del_.Init();
    lfo_amp_  = 0.f;
    feedback_ = .2f;
    SetFreq(200.f);

    del_.SetDelay(0.f);

    os_ = 30.f; //30 hertz frequency offset, lower than this introduces crunch
    deltime_ = 0.f;

    last_sample_ = 0.f;
    lfo_phase_   = 0.f;
    SetLfoFreq(.3);
    SetLfoDepth(.9);
}

float PhaserEngine::Process(float in)
{
    float lfo_sig = ProcessLfo();
    fonepole(deltime_, sample_rate_ / (lfo_sig + ap_freq_ + os_), .0001f);

    last_sample_ = del_.Allpass(in + feedback_ * last_sample_, deltime_, .3f);

    return (in + last_sample_) * .5f; //equal mix
}

void PhaserEngine::SetLfoDepth(float depth)
{
    lfo_amp_ = fclamp(depth, 0.f, 1.f);
}

void PhaserEngine::SetLfoFreq(float lfo_freq)
{
    lfo_freq = 4.f * lfo_freq / sample_rate_;
    lfo_freq *= lfo_freq_ < 0.f ? -1.f : 1.f; //if we're headed down, keep going
    lfo_freq_ = fclamp(lfo_freq, -.25f, .25f); //clip at +/- .125 * sr
}

void PhaserEngine::SetFreq(float ap_freq)
{
    ap_freq_ = fclamp(ap_freq, 0.f, 20000.f); //0 - 20kHz
}

void PhaserEngine::SetFeedback(float feedback)
{
    feedback_ = fclamp(feedback, 0.f, .75f);
}

float PhaserEngine::ProcessLfo()
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

    return lfo_phase_ * lfo_amp_ * ap_freq_;
}

//Phaser Stuff
void Phaser::Init(float sample_rate)
{
    //    engines_[0].Init(sample_rate);
    //    engines_[1].Init(sample_rate);

    for(size_t i = 0; i < kMaxPoles; i++)
        apf_[i].Init(sample_rate);
    //        engines_[i].Init(sample_rate);

    poles_     = 4;
    gain_frac_ = .5f;
    sr_        = sample_rate;
    rsr_       = 1.f / sr_;
}

float Phaser::Process(float in)
{
    float s;
    float base_freq = freq_;
    float sig;
    sig = 0.f;
    for(size_t i = 0; i < poles_; i++)
    {
        float freq = base_freq * (float)(i + 1);
        if(i == 0)
            s = in + last_ * feedback_;
        else
            s = sig;
        //        //float c = freq * cot(freq * 48000.f / 2);
        //        float c, d;
        //        float theta;
        //        theta = (PI_F * freq) * rsr_;
        //        d     = tan(theta);
        //        c     = (1.f - d) / (1 + d);
        //        //        c = (1 - tan(x)) / (1 + tan(x));
        //        c_[i] = c;

        //sig += apf_[i].Allpass(s, 1, c_[i]);
        apf_[i].SetFreq(freq);
        c_[i] = apf_[i].GetCoeff();
        pfreq_[i] = freq;
        sig = apf_[i].Process(s);
    }

    //    for(int i = 0; i < poles_; i++)
    //    {
    //        sig += engines_[i].Process(in);
    //    }
    last_ = sig;

    return sig + in;
}

void Phaser::SetPoles(int poles)
{
    poles_ = DSY_CLAMP(poles, 1, 8);
}

void Phaser::SetLfoDepth(float depth)
{
    //    for(int i = 0; i < kMaxPoles; i++)
    //    {
    //        engines_[i].SetLfoDepth(depth);
    //    }
}

void Phaser::SetLfoFreq(float lfo_freq)
{
    //    for(int i = 0; i < kMaxPoles; i++)
    //    {
    //        engines_[i].SetLfoFreq(lfo_freq);
    //    }
}

void Phaser::SetFreq(float ap_freq)
{
    //    for(int i = 0; i < kMaxPoles; i++)
    //    {
    //        engines_[i].SetFreq(ap_freq);
    //    }
    freq_ = ap_freq;
}

void Phaser::SetFeedback(float feedback)
{
    //    for(int i = 0; i < kMaxPoles; i++)
    //    {
    //        engines_[i].SetFeedback(feedback);
    //    }
    feedback_ = feedback;
}