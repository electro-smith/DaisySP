// #adsr
// adsr envelope module
// ## Credits
// *Original author(s) : Paul Batchelor
// *Ported from Soundpipe by Ben Sergentanis, May 2020

#pragma once
#ifndef DSY_ADSR_H
#define DSY_ADSR_H

#include <stdint.h>
#ifdef __cplusplus

namespace daisysp
{

// ### Envelope Segments
//
// Distinct stages that the phase of the envelope can be located in.
//
// - IDLE   = located at phase location 0, and not currently running
// - ATTACK  = First segment of envelope where phase moves from 0 to 1
// - DECAY   = Second segment of envelope where phase moves from 1 to SUSTAIN value
// - SUSTAIN = Third segment of envelope, stays at SUSTAIN level until GATE is released
// - RELEASE = 	Fourth segment of envelop where phase moves from SUSTAIN to 0
// - LAST    =  Last segment, aka release
// ~~~~
enum
{
    ADSR_SEG_IDLE,
    ADSR_SEG_ATTACK,
    ADSR_SEG_DECAY,
    ADSR_SEG_SUSTAIN,
    ADSR_SEG_RELEASE,
    ADSR_SEG_LAST,
};
// ~~~~


class Adsr
{
    public:
    Adsr() {}
    ~Adsr() {}
  
    // ### Init
    //
    // Initializes the ATone module.
    //
    // sample_rate - The sample rate of the audio engine being run. 
    // 
    // ~~~~
        void Init(float sample_rate);
    // ~~~~

    // ### Process
    //
    // Processes one sample through the filter and returns one sample.
    //
    // gate - trigger the envelope, hold it to sustain 
    // 
    // ~~~~
        float Process(bool gate);
    // ~~~~
  
    // ## Setters
    // 
    // ### SetTime
    //
    // Set time per segment in seconds
    //		
    // ~~~~
        inline void SetTime(int seg, float time) { seg_time_[seg]  = time; }
    // ~~~~

    // ### SetSustainLevel
    //
    // Arguments float sus_level, sets sustain level
    //		
    // ~~~~
        inline void SetSustainLevel (float sus_level) {  sus_ = sus_level; }
    // ~~~~

    // ## Getters
    //
    //		
    // ### GetCurrentSegment
    //
    // Returns the segment of the envelope that the phase is currently located in.
    // ~~~~
        inline uint8_t GetCurrentSegment() { return mode_; }
    // ~~~~

    // ### IsRunning
    // Returns true if the envelope is currently in any stage apart from idle.
    // ~~~~
        inline bool IsRunning() const
    // ~~~~
    {
        return mode_ != ADSR_SEG_IDLE;
    }
    
    private:
	float  sus_, seg_time_[ADSR_SEG_LAST],
	       a_, b_, y_, x_, prev_, atk_time_;
	int sample_rate_;
	uint8_t mode_;
	float Tau2Pole(float tau);
	float AdsrFilter();
    };
} // namespace daisysp
#endif
#endif
