#pragma once
#include <algorithm>
#include "dsp.h"

namespace daisysp
{
/** Multimode audio looper
* 
* Modes are:
*  - Normal
*  - Onetime Dub
*  - Replace
*  - Frippertronics
*
* Read more about the looper modes in the mode enum documentation.
*/
class Looper
{
  public:
    Looper() {}
    ~Looper() {}

    /** 
     ** Normal Mode: Input is added to the existing loop infinitely while recording 
     ** 
     ** Onetime Dub Mode: Recording starts at the first sample of the buffer and is added
     **     to the existing buffer contents. Recording automatically stops after one full loop.
     **
     ** Replace Mode: Audio in the buffer is replaced while recording is on.
     **
     ** Frippertronics Mode: infinite looping recording with fixed decay on each loop. The module acts like tape-delay set up.
     */
    enum class Mode
    {
        NORMAL,
        ONETIME_DUB,
        REPLACE,
        FRIPPERTRONICS,
    };

    void Init(float *mem, size_t size)
    {
        buffer_size_ = size;
        buff_        = mem;

        InitBuff();
        state_      = State::EMPTY;
        mode_       = Mode::NORMAL;
        half_speed_ = false;
        reverse_    = false;
        rec_queue_  = false;
        win_idx_    = 0;
    }

    /** Handles reading/writing to the Buffer depending on the mode. */
    float Process(const float input)
    {
        float sig = 0.f;
        float inc;
        bool  hitloop = false;
        // Record forward at normal speed during the first loop no matter what.
        inc = state_ == State::EMPTY || state_ == State::REC_FIRST
                  ? 1.f
                  : GetIncrementSize();
        win_ = WindowVal(win_idx_ * kWindowFactor);
        switch(state_)
        {
            case State::EMPTY: sig = 0.0f; break;
            case State::REC_FIRST:
                sig = 0.f;
                Write(pos_, input * win_);
                if(win_idx_ < kWindowSamps - 1)
                    win_idx_ += 1;
                recsize_ = pos_;
                pos_ += inc;
                if(pos_ > buffer_size_ - 1)
                {
                    state_   = State::PLAYING;
                    recsize_ = pos_ - 1;
                    pos_     = 0;
                }
                break;
            case State::PLAYING:
                sig = Read(pos_);
                /** This is a way of 'seamless looping'
				 ** The first N samps after recording is done are recorded with the input faded out. 
				 */
                if(win_idx_ < kWindowSamps - 1)
                {
                    Write(pos_, sig + input * (1.f - win_));
                    win_idx_ += 1;
                }

                pos_ += inc;
                if(pos_ > recsize_ - 1)
                {
                    pos_    = 0;
                    hitloop = true;
                }
                else if(pos_ < 0)
                {
                    pos_    = recsize_ - 1;
                    hitloop = true;
                }
                if(hitloop)

                {
                    if(rec_queue_ && mode_ == Mode::ONETIME_DUB)
                    {
                        rec_queue_ = false;
                        state_     = State::REC_DUB;
                        win_idx_   = 0;
                    }
                }
                break;
            case State::REC_DUB:
                sig = Read(pos_);
                switch(mode_)
                {
                    case Mode::REPLACE: Write(pos_, input * win_); break;
                    case Mode::FRIPPERTRONICS:
                        Write(pos_, (input * win_) + (sig * kFripDecayVal));
                        break;
                    case Mode::NORMAL:
                    case Mode::ONETIME_DUB:
                    default: Write(pos_, (input * win_) + sig); break;
                }
                if(win_idx_ < kWindowSamps - 1)
                    win_idx_ += 1;
                pos_ += inc;
                if(pos_ > recsize_ - 1)
                {
                    pos_    = 0;
                    hitloop = true;
                }
                else if(pos_ < 0)
                {
                    pos_    = recsize_ - 1;
                    hitloop = true;
                }
                if(hitloop && mode_ == Mode::ONETIME_DUB)
                {
                    state_   = State::PLAYING;
                    win_idx_ = 0;
                }

                break;
            default: break;
        }
        near_beginning_ = state_ != State::EMPTY && !Recording() && pos_ < 4800
                              ? true
                              : false;

        return sig;
    }

    /** Effectively erases the buffer 
     ** Note: This does not actually change what is in the buffer  */
    inline void Clear() { state_ = State::EMPTY; }

    /** Engages/Disengages the recording, depending on Mode.
     ** In all modes, the first time this is triggered a new loop will be started.
     ** The second trigger will set the loop size, and begin playback of the loop. 
    */
    inline void TrigRecord()
    {
        switch(state_)
        {
            case State::EMPTY:
                pos_        = 0;
                recsize_    = 0;
                state_      = State::REC_FIRST;
                half_speed_ = false;
                reverse_    = false;
                break;
            case State::REC_FIRST:
            case State::REC_DUB: state_ = State::PLAYING; break;
            case State::PLAYING:
                if(mode_ == Mode::ONETIME_DUB)
                    rec_queue_ = true;
                else
                    state_ = State::REC_DUB;
                break;
            default: state_ = State::EMPTY; break;
        }
        if(!rec_queue_)
            win_idx_ = 0;
    }

    /** Returns true if the looper is currently being written to. */
    inline const bool Recording() const
    {
        return state_ == State::REC_DUB || state_ == State::REC_FIRST;
    }

    inline const bool RecordingQueued() const { return rec_queue_; }

    /** Increments the Mode by one step useful for buttons, etc. that need to step through the Looper modes. */
    inline void IncrementMode()
    {
        int m = static_cast<int>(mode_);
        m     = m + 1;
        if(m > kNumModes - 1)
            m = 0;
        mode_ = static_cast<Mode>(m);
    }

    /** Sets the recording mode to the specified Mode. */
    inline void SetMode(Mode mode) { mode_ = mode; }

    /** Returns the specific recording mode that is currently set. */
    inline const Mode GetMode() const { return mode_; }

    inline void ToggleReverse() { reverse_ = !reverse_; }
    inline void SetReverse(bool state) { reverse_ = state; }
    inline bool GetReverse() const { return reverse_; }

    inline void ToggleHalfSpeed() { half_speed_ = !half_speed_; }
    inline void SetHalfSpeed(bool state) { half_speed_ = state; }
    inline bool GetHalfSpeed() const { return half_speed_; }

    inline bool IsNearBeginning() { return near_beginning_; }

  private:
    /** Constants */

    /** Decay value for frippertronics mode is sin(PI / 4) */
    static constexpr float kFripDecayVal      = 0.7071067811865476f;
    static constexpr int   kNumModes          = 4;
    static constexpr int   kNumPlaybackSpeeds = 3;
    static constexpr int   kWindowSamps       = 1200;
    static constexpr float kWindowFactor      = (1.f / kWindowSamps);

    /** Private Member Functions */
    float GetIncrementSize()
    {
        float inc = 1.f;
        if(half_speed_)
            inc = 0.5f;
        return reverse_ ? -inc : inc;
    }

    /** Initialize the buffer */
    void InitBuff() { std::fill(&buff_[0], &buff_[buffer_size_ - 1], 0); }

    /** Get a floating point sample from the buffer */
    inline const float Read(size_t pos) const { return buff_[pos]; }

    /** Reads from a specified point in the delay line using linear interpolation */
    float ReadF(float pos)
    {
        float    a, b, frac;
        uint32_t i_idx = static_cast<uint32_t>(pos);
        frac           = pos - i_idx;
        a              = buff_[i_idx];
        b              = buff_[(i_idx + 1) % buffer_size_];
        return a + (b - a) * frac;
    }

    /** Write to a known location in the buffer */
    inline void Write(size_t pos, float val) { buff_[pos] = val; }

    /** Linear to Constpower approximation for windowing*/
    float WindowVal(float in) { return sin(HALFPI_F * in); }

    // Private Enums

    /** Internal looper state */
    enum class State
    {
        EMPTY,
        REC_FIRST,
        PLAYING,
        REC_DUB,
    };

    /** Private Member Variables */
    Mode   mode_;
    State  state_;
    float *buff_;
    size_t buffer_size_;
    float  pos_, win_;
    size_t win_idx_;
    bool   half_speed_;
    bool   reverse_;
    size_t recsize_;
    bool   rec_queue_;
    bool   near_beginning_;
};

} // namespace daisysp
