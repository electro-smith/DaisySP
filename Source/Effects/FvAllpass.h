#pragma once
#ifndef DSY_FV_ALLPASS_H
#define DSY_FV_ALLPASS_H
#include "Utility/dsp.h"


namespace daisysp
{
/** Allpass utility block for Freeverb. 
    Consider merging with previous allpass or replace if functionally similar
    */
class FvAllpass
{
  public:
    FvAllpass() {}
    ~FvAllpass() {}

    void Init(float *buf, size_t size)
    {
        buffer_ = buf;
        size_   = size;
        idx_    = 0;
        Mute();
        fb_ = 0.7f;
    }

    inline float Process(float input)
    {
        float output, bufout;
        output = 0.f;
        bufout = buffer_[idx_];
        denormalize(bufout);
        output        = -input + bufout;
        buffer_[idx_] = input + (bufout * fb_);
        idx_ += 1;
        if(idx_ >= size_)
            idx_ = 0;
        return output;
    }

    /** Clears internal buffer */
    void Mute()
    {
        for(int i = 0; i < size_; i++)
        {
            buffer_[i] = 0.f;
        }
    }

    /*** Sets the amount of feedback through the allpass */
    void SetFeedback(float val) { fb_ = val; }

    /** Returns the current feedback value. */
    const inline float GetFeedback() const { return fb_; }


  private:
    float  fb_;
    float *buffer_;
    int    size_, idx_;
};

} // namespace daisysp


#endif
