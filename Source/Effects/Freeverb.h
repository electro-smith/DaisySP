#pragma once
#ifndef DSY_FREEVERB_H
#define DSY_FREEVERB_H

#include "FvAllpass.h"
#include "FvComb.h"

namespace daisysp
{
/** Tuning Specific settings -- should be configurable */
/** Considering either making this a separate header or just a config struct that could be used as a template parameter */

const int   kNumCombs   = 8;
const int   kNumAllpass = 4;
const float kMuted      = 0;
const float kFixedGain  = 0.015f;
const float kScaleWet     = 3;
//const float kScaleWet     = 4;
const float kScaleDry     = 2;
const float kScaledamp    = 0.4f;
const float kScaleroom    = 0.28f;
const float kOffsetroom   = 0.7f;
const float kInitialroom  = 0.5f;
const float kInitialdamp  = 0.5f;
const float kInitialwet   = 1 / kScaleWet;
const float kInitialdry   = 0;
const float kInitialwidth = 1;
const float kInitialmode  = 0;
const float kFreezeMode   = 0.5f;
const int   kStereoSpread = 23;

constexpr float kTargetSr    = 48000.f;
constexpr float kOriginalSr  = 44100.f;
constexpr float kRetuneCoeff = (kTargetSr / kOriginalSr);

// These values assume 44.1KHz sample rate
// they will probably be OK for 48KHz sample rate
// but would need scaling for 96KHz (or other) sample rates.
// The values were obtained by listening tests.

/** Freeverb Implementation 
  * based on original C++ sources from  Jezar at Dreampoint, June 2000 
  * 
  */
class Freeverb
{
  public:
    Freeverb() {}
    ~Freeverb() {}
    /** Initialize the internal bits */
    void Init();

    /** Clear all of the filters and stop the reverb. */
    void Mute();

    /** Process a single sample replacing the output */
    void Process(float *inl, float *inr, float *outl, float *outr);

    /** Processes a block of audio, skip can be used if necessary. 
        The contents of *outN are replaced by the new signal  */
    void ProcessBlockReplace(float *inl,
                             float *inr,
                             float *outl,
                             float *outr,
                             int    size,
                             int    skip = 1);

    /** Processes a block of audio, skip can be used if necessary. 
        The contents of *outN are mixed with by the new signal  */
    void ProcessBlockMix(float *inl,
                         float *inr,
                         float *outl,
                         float *outr,
                         int    size,
                         int    skip = 1);

    void SetRoomSize(float val)
    {
        roomsize_ = (val * kScaleroom) + kOffsetroom;
        Update();
    }
    inline float GetRoomSize()
    {
        return (roomsize_ - kOffsetroom) / kScaleroom;
    }

    void SetDamp(float val)
    {
        damp_ = val * kScaledamp;
        Update();
    }
    const inline float GetDamp() const { return damp_ / kScaledamp; }

    void SetWet(float val)
    {
        wet_ = val * kScaleWet;
        Update();
    }
    const inline float GetWet() const { return wet_ / kScaleWet; }

    void SetDry(float val)
    {
        dry_ = val * kScaleDry;
        Update();
    }
    const inline float GetDry() const { return dry_ / kScaleDry; }

    void SetWidth(float val)
    {
        width_ = val;
        Update();
    }

    const inline float GetWidth() const { return width_; }

    void SetMode(float val)
    {
        mode_ = val;
        Update();
    }
    const inline float GetMode() const
    {
        return mode_ > kFreezeMode ? 1.f : 0.f;
    }


  private:
    static constexpr int kCombTuningL1 = 1116 * kRetuneCoeff;
    static constexpr int kCombTuningR1 = (1116 + kStereoSpread) * kRetuneCoeff;
    static constexpr int kCombTuningL2 = 1188 * kRetuneCoeff;
    static constexpr int kCombTuningR2 = (1188 + kStereoSpread) * kRetuneCoeff;
    static constexpr int kCombTuningL3 = 1277 * kRetuneCoeff;
    static constexpr int kCombTuningR3 = (1277 + kStereoSpread) * kRetuneCoeff;
    static constexpr int kCombTuningL4 = 1356 * kRetuneCoeff;
    static constexpr int kCombTuningR4 = (1356 + kStereoSpread) * kRetuneCoeff;
    static constexpr int kCombTuningL5 = 1422 * kRetuneCoeff;
    static constexpr int kCombTuningR5 = (1422 + kStereoSpread) * kRetuneCoeff;
    static constexpr int kCombTuningL6 = 1491 * kRetuneCoeff;
    static constexpr int kCombTuningR6 = (1491 + kStereoSpread) * kRetuneCoeff;
    static constexpr int kCombTuningL7 = 1557 * kRetuneCoeff;
    static constexpr int kCombTuningR7 = (1557 + kStereoSpread) * kRetuneCoeff;
    static constexpr int kCombTuningL8 = 1617 * kRetuneCoeff;
    static constexpr int kCombTuningR8 = (1617 + kStereoSpread) * kRetuneCoeff;
    static constexpr int kAllpassTuningL1 = 556 * kRetuneCoeff;
    static constexpr int kAllpassTuningR1
        = (556 + kStereoSpread) * kRetuneCoeff;
    static constexpr int kAllpassTuningL2 = 441 * kRetuneCoeff;
    static constexpr int kAllpassTuningR2
        = (441 + kStereoSpread) * kRetuneCoeff;
    static constexpr int kAllpassTuningL3 = 341 * kRetuneCoeff;
    static constexpr int kAllpassTuningR3
        = (341 + kStereoSpread) * kRetuneCoeff;
    static constexpr int kAllpassTuningL4 = 225 * kRetuneCoeff;
    static constexpr int kAllpassTuningR4
        = (225 + kStereoSpread) * kRetuneCoeff;

    void  Update();
    float gain_, roomsize_, roomsize1_;
    float damp_, damp1_;
    float wet_, wet1_, wet2_;
    float dry_;
    float width_, mode_;

    FvComb    combL[kNumCombs], combR[kNumCombs];
    FvAllpass allpassL[kNumAllpass], allpassR[kNumAllpass];

    /** Buffers for combs */
    float bufcombL1[kCombTuningL1];
    float bufcombR1[kCombTuningR1];
    float bufcombL2[kCombTuningL2];
    float bufcombR2[kCombTuningR2];
    float bufcombL3[kCombTuningL3];
    float bufcombR3[kCombTuningR3];
    float bufcombL4[kCombTuningL4];
    float bufcombR4[kCombTuningR4];
    float bufcombL5[kCombTuningL5];
    float bufcombR5[kCombTuningR5];
    float bufcombL6[kCombTuningL6];
    float bufcombR6[kCombTuningR6];
    float bufcombL7[kCombTuningL7];
    float bufcombR7[kCombTuningR7];
    float bufcombL8[kCombTuningL8];
    float bufcombR8[kCombTuningR8];

    // Buffers for the allpasses
    float bufallpassL1[kAllpassTuningL1];
    float bufallpassR1[kAllpassTuningR1];
    float bufallpassL2[kAllpassTuningL2];
    float bufallpassR2[kAllpassTuningR2];
    float bufallpassL3[kAllpassTuningL3];
    float bufallpassR3[kAllpassTuningR3];
    float bufallpassL4[kAllpassTuningL4];
    float bufallpassR4[kAllpassTuningR4];
};

} // namespace daisysp

#endif