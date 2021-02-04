#pragma once
#ifndef DSY_POLYPLUCK_H
#define DSY_POLYPLUCK_H

#include <stdlib.h>
#include "Utility/dsp.h"
#include "PhysicalModeling/pluck.h"
#include "Utility/dcblock.h"

namespace daisysp
{
/** Simplified Pseudo-Polyphonic Pluck Voice

Template Based Pluck Voice, with configurable number of voices and simple pseudo-polyphony.

DC Blocking included to prevent biases from causing unwanted saturation distortion.

**Author**: shensley

**Date Added**: March 2020
*/
template <size_t num_voices>
class PolyPluck
{
  public:
    /** Initializes the PolyPluck instance.
        \param sample_rate: rate in Hz that the Process() function will be called.
    */
    void Init(float sample_rate)
    {
        active_voice_ = 0;
        p_damp_       = 0.95f;
        p_decay_      = 0.75f;
        for(size_t i = 0; i < num_voices; i++)
        {
            plk_[i].Init(sample_rate, plkbuff_[i], 256, PLUCK_MODE_RECURSIVE);
            plk_[i].SetDamp(0.85f);
            plk_[i].SetAmp(0.18f);
            plk_[i].SetDecay(0.85f);
        }
        blk_.Init(sample_rate);
    }

    /** Process function, synthesizes and sums the output of all voices,
        triggering a new voice with frequency of MIDI note number when trig > 0.

        \param trig: value by reference of trig. When trig > 0 a the next voice will be triggered, and trig will be set to 0.
        \param note: MIDI note number for the active_voice.
    */
    float Process(float &trig, float note)
    {
        float sig, tval;
        sig = 0.0f;
        if(trig > 0.0f)
        {
            // increment active voice
            active_voice_ = (active_voice_ + 1) % num_voices;
            // set new voice to new note
            plk_[active_voice_].SetDamp(p_damp_);
            plk_[active_voice_].SetDecay(p_decay_);
            plk_[active_voice_].SetAmp(0.25f);
        }
        plk_[active_voice_].SetFreq(mtof(note));

        for(size_t i = 0; i < num_voices; i++)
        {
            tval = (trig > 0.0f && i == active_voice_) ? 1.0f : 0.0f;
            sig += plk_[i].Process(tval);
        }
        if(trig > 0.0f)
            trig = 0.0f;
        return blk_.Process(sig);
    }

    /** Sets the decay coefficients of the pluck voices. 
        \param p expects 0.0-1.0 input.
    */
    void SetDecay(float p) { p_damp_ = p; }

  private:
    DcBlock blk_;
    Pluck   plk_[num_voices];
    float   plkbuff_[num_voices][256];
    float   p_damp_, p_decay_;
    size_t  active_voice_;
};

} // namespace daisysp

#endif
