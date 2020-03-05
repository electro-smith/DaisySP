// # adenv
//
// Author: shensley
//
// Trigger-able envelope with adjustable min/max, and independent per-segment time control.
//
// TODO:
//
// - Add Cycling
// - Implement Curve (its only linear for now).
// - Maybe make this an ADsr_ that has AD/AR/Asr_ modes.

#pragma once
#ifndef ADENV_H
#define ADENV_H
#include <stdint.h>
#ifdef __cplusplus

namespace daisysp
{
// ### Envelope Segments
//
// Distinct stages that the phase of the envelope can be located in.
//
// - IDLE = located at phase location 0, and not currently running
// - ATTACK = First segment of envelope where phase moves from MIN value to MAX value
// - DECAY = Second segment of envelope where phase moves from MAX to MIN value
// - LAST = The final segment of the envelope (currently decay)
// ~~~~
enum
{
    ADENV_SEG_IDLE,
    ADENV_SEG_ATTACK,
    ADENV_SEG_DECAY,
    ADENV_SEG_LAST,
};
// ~~~~

class AdEnv
{
  public:
    AdEnv() {}
    ~AdEnv() {}

    // ### Init
    //
    // Initializes the ad envelope
    //
    // float sample_rate - sample rate of the audio engine being run.
    //
    // Defaults
    //
    // - current segment = idle
    // - curve = linear
    // - phase = 0
    // - min = 0
    // - max = 1
    //
    // ~~~~
    void Init(float sample_rate);
    // ~~~~

    // ### Process
    // processes the current sample of the envelope. Returns the current envelope value. This should be called once per sample period.
    // ~~~~
    float Process();
    // ~~~~

    // ### Trigger
    // Starts or retriggers the envelope.
    // ~~~~
    inline void Trigger() { trigger_ = 1; }
    // ~~~~

    // ## Mutators

    // ### SetTime
    // Sets the length of time (in seconds) for a specific segment.
    // ~~~~
    inline void SetTime(uint8_t seg, float time)
    // ~~~~
    {
        segment_time_[seg] = time;
    }

    // ### SetCurve
    // Sets the amount of curve applied. A positve value will create a log
    // Input range: -100 to 100.  (or more)
    //
    // ~~~~
    inline void SetCurve(float scalar) { curve_scalar_ = scalar; }
    // ~~~~

    // ### SetMin
    //
    // Sets the minimum value of the envelope output
    // Input range: -FLTmax_, to FLTmax_
    // ~~~~
    inline void SetMin(float min) { min_ = min; }
    // ~~~~

    // ### SetMax
    //
    // Sets the maximum value of the envelope output
    // Input range: -FLTmax_, to FLTmax_
    // ~~~~
    inline void SetMax(float max) { max_ = max; }
    // ~~~~

    // ## Accessors
    // ### GetValue
    // Returns the current output value without processing the next sample
    inline float GetValue() const { return (output_ * (max_ - min_)) + min_; }

    // ### GetCurrentSegment
    //
    // Returns the segment of the envelope that the phase is currently located in.
    // ~~~~
    inline uint8_t GetCurrentSegment() { return current_segment_; }
    // ~~~~
    //

    // ### IsRunning
    // Returns true if the envelope is currently in any stage apart from idle.
    // ~~~~
    inline bool IsRunning() const
    // ~~~~
    {
        return current_segment_ != ADENV_SEG_IDLE;
    }

  private:
    uint8_t  current_segment_, prev_segment_;
    float    segment_time_[ADENV_SEG_LAST];
    float    sample_rate_, min_, max_, output_, curve_scalar_;
    float    c_inc_, curve_x_, retrig_val_;
    uint32_t phase_;
    uint8_t  trigger_;
};

} // namespace daisysp
#endif
#endif
