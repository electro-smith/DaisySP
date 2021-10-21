#pragma once
#ifndef DSY_FV_COMB_H
#define DSY_FV_COMB_H
#include "Utility/dsp.h"


namespace daisysp
{
/** Comb utility block for Freeverb. 
    Consider merging with previous comb or replace if functionally similar
    Lowpass Feedback Comb Filter
    */
class FvComb
{
  public:
    FvComb() {}
    ~FvComb() {}

    void Init(float *buf, int size)
    {
        buffer_ = buf;
        size_   = size;
        Mute();
        idx_   = 0;
        fb_    = 0.5f;
        prev_  = 0.f;
        damp1_ = 0.3f;
        damp2_ = 0.7f;
    }

    void Mute()
    {
        for(int i = 0; i < size_; i++)
        {
            buffer_[i] = 0.f;
        }
    }

    inline float Process(float input)
    {
        float output;
        output = buffer_[idx_];
        denormalize(output);
        prev_ = (output * damp2_) + (prev_ * damp1_);
        denormalize(prev_);
        buffer_[idx_] = input + (prev_ * fb_);
        idx_ += 1;
        if(idx_ >= size_)
            idx_ = 0;
        return output;
    }

    /*** Sets the amount of feedback through the allpass */
    void SetFeedback(float val) { fb_ = val; }

    /** Returns the current feedback value. */
    const inline float GetFeedback() const { return fb_; }

    /*** Sets the amount of feedback through the allpass */
    void SetDamp(float val)
    {
        damp1_ = val;
        damp2_ = 1.f - val;
    }

    /** Returns the current feedback value. */
    const inline float GetDamp() const { return damp1_; }

  private:
    float  fb_, prev_, damp1_, damp2_;
    float *buffer_;
    int    size_, idx_;
};

} // namespace daisysp


#endif
