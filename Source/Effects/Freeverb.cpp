#include "Freeverb.h"


using namespace daisysp;

void Freeverb::Init()
{
    /** Init all the buffs */
    combL[0].Init(bufcombL1, kCombTuningL1);
    combR[0].Init(bufcombR1, kCombTuningR1);
    combL[1].Init(bufcombL2, kCombTuningL2);
    combR[1].Init(bufcombR2, kCombTuningR2);
    combL[2].Init(bufcombL3, kCombTuningL3);
    combR[2].Init(bufcombR3, kCombTuningR3);
    combL[3].Init(bufcombL4, kCombTuningL4);
    combR[3].Init(bufcombR4, kCombTuningR4);
    combL[4].Init(bufcombL5, kCombTuningL5);
    combR[4].Init(bufcombR5, kCombTuningR5);
    combL[5].Init(bufcombL6, kCombTuningL6);
    combR[5].Init(bufcombR6, kCombTuningR6);
    combL[6].Init(bufcombL7, kCombTuningL7);
    combR[6].Init(bufcombR7, kCombTuningR7);
    combL[7].Init(bufcombL8, kCombTuningL8);
    combR[7].Init(bufcombR8, kCombTuningR8);
    allpassL[0].Init(bufallpassL1, kAllpassTuningL1);
    allpassR[0].Init(bufallpassR1, kAllpassTuningR1);
    allpassL[1].Init(bufallpassL2, kAllpassTuningL2);
    allpassR[1].Init(bufallpassR2, kAllpassTuningR2);
    allpassL[2].Init(bufallpassL3, kAllpassTuningL3);
    allpassR[2].Init(bufallpassR3, kAllpassTuningR3);
    allpassL[3].Init(bufallpassL4, kAllpassTuningL4);
    allpassR[3].Init(bufallpassR4, kAllpassTuningR4);

    // Set default values
    allpassL[0].SetFeedback(0.5f);
    allpassR[0].SetFeedback(0.5f);
    allpassL[1].SetFeedback(0.5f);
    allpassR[1].SetFeedback(0.5f);
    allpassL[2].SetFeedback(0.5f);
    allpassR[2].SetFeedback(0.5f);
    allpassL[3].SetFeedback(0.5f);
    allpassR[3].SetFeedback(0.5f);
    SetWet(kInitialwet);
    SetRoomSize(kInitialroom);
    SetDry(kInitialdry);
    SetDamp(kInitialdamp);
    SetWidth(kInitialwidth);
    SetMode(kInitialmode);
    // Clear the buffers
    Mute();
}
void Freeverb::Mute()
{
    if(GetMode() >= kFreezeMode)
        return;
    for(int i = 0; i < kNumCombs; i++)
    {
        combL[i].Mute();
        combR[i].Mute();
    }
    for(int i = 0; i < kNumAllpass; i++)
    {
        allpassL[i].Mute();
        allpassR[i].Mute();
    }
}
void Freeverb::Process(float *inl, float *inr, float *outl, float *outr)
{
    float left, right, input;

    left = right = 0.f;
    input        = (*inl + *inr) * gain_;

    // Parallel comb accumulation
    for(int i = 0; i < kNumCombs; i++)
    {
        left += combL[i].Process(input);
        right += combR[i].Process(input);
    }
    // Series allpass
    for(int i = 0; i < kNumAllpass; i++)
    {
        left  = allpassL[i].Process(left);
        right = allpassR[i].Process(right);
    }
    // Calc replacement
    *outl = left * wet1_ + right * wet2_ + *inl * dry_;
    *outr = right * wet1_ + left * wet2_ + *inr * dry_;
}
void Freeverb::ProcessBlockReplace(float *inl,
                                   float *inr,
                                   float *outl,
                                   float *outr,
                                   int    size,
                                   int    skip)
{
    float left, right, input;
    while(size--)
    {
        left = right = 0.f;
        input        = (*inl + *inr) * gain_;

        // Parallel comb accumulation
        for(int i = 0; i < kNumCombs; i++)
        {
            left += combL[i].Process(input);
            right += combR[i].Process(input);
        }
        // Series allpass
        for(int i = 0; i < kNumAllpass; i++)
        {
            left  = allpassL[i].Process(left);
            right = allpassR[i].Process(right);
        }
        // Calc replacement
        *outl = left * wet1_ + right * wet2_ + *inl * dry_;
        *outr = right * wet1_ + left * wet2_ + *inr * dry_;
        // Increment buffers
        *inl += skip;
        *inr += skip;
        *outl += skip;
        *outr += skip;
    }
}
void Freeverb::ProcessBlockMix(float *inl,
                               float *inr,
                               float *outl,
                               float *outr,
                               int    size,
                               int    skip)
{
    float left, right, input;
    while(size--)
    {
        left = right = 0.f;
        input        = (*inl + *inr) * gain_;

        // Parallel comb accumulation
        for(int i = 0; i < kNumCombs; i++)
        {
            left += combL[i].Process(input);
            right += combR[i].Process(input);
        }
        // Series allpass
        for(int i = 0; i < kNumAllpass; i++)
        {
            left  = allpassL[i].Process(left);
            right = allpassR[i].Process(right);
        }
        // Calcmixin
        *outl = left * wet1_ + right * wet2_ + *inl * dry_;
        *outr = right * wet1_ + left * wet2_ + *inr * dry_;
        // Increment buffers
        *inl += skip;
        *inr += skip;
        *outl += skip;
        *outr += skip;
    }
}

void Freeverb::Update()
{
    wet1_ = wet_ * (width_ / 2.f + 0.5f);
    if(mode_ > kFreezeMode)
    {
        roomsize1_ = 1.f;
        damp1_     = 0;
        gain_      = kMuted;
    }
    else
    {
        roomsize1_ = roomsize_;
        damp1_     = damp_;
        gain_      = kFixedGain;
    }
    for(int i = 0; i < kNumCombs; i++)
    {
        combL[i].SetFeedback(roomsize1_);
        combR[i].SetFeedback(roomsize1_);
    }
    for(int i = 0; i < kNumCombs; i++)
    {
        combL[i].SetDamp(damp1_);
        combR[i].SetDamp(damp1_);
    }
}
