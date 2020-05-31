
#pragma once
#ifndef DSY_PLUCK_H
#define DSY_PLUCK_H

#include <stdint.h>
#ifdef __cplusplus

namespace daisysp
{
/** The method of natural decay that the algorithm will use.
    - RECURSIVE: 1st order recursive filter, with coefs .5.
    - WEIGHTED_AVERAGE: weighted averaging. 
    */
enum
{
    PLUCK_MODE_RECURSIVE,
    PLUCK_MODE_WEIGHTED_AVERAGE,
    PLUCK_LAST,
};

/** Produces a naturally decaying plucked string or drum sound based on the Karplus-Strong algorithms.

    Ported from soundpipe to DaisySP
    
    This code was originally extracted from the Csound opcode "pluck"

    Original Author(s): Barry Vercoe, John ffitch
    Year: 1991

    Location: OOps/ugens4.c
    */
class Pluck
{
  public:
    Pluck() {}
    ~Pluck() {}
    /** Initializes the Pluck module.

            \param sample_rate: Sample rate of the audio engine being run.
            \param buf: buffer used as an impulse when triggering the Pluck algorithm
            \param npt: number of elementes in buf.
            \param mode: Sets the mode of the algorithm.
    */
    void Init(float sample_rate, float *buf, int32_t npt, int32_t mode);


    /** Processes the waveform to be generated, returning one sample. This should be called once per sample period.
    */
    float Process(float &trig);

    /** 
        Sets the amplitude of the output signal.
        Input range: 0-1?
    */
    inline void SetAmp(float amp) { amp_ = amp; }
    /** Sets the frequency of the output signal in Hz.
        Input range: Any positive value
    */
    inline void SetFreq(float freq) { freq_ = freq; }
    /** Sets the time it takes for a triggered note to end in seconds.
        Input range: 0-1
    */
    inline void SetDecay(float decay) { decay_ = decay; }
    /** Sets the dampening factor applied by the filter (based on PLUCK_MODE)
        Input range: 0-1
    */
    inline void SetDamp(float damp) { damp_ = damp; }
    /** Sets the mode of the algorithm.
    */
    inline void SetMode(int32_t mode) { mode_ = mode; }
    /** Returns the current value for amp.
    */
    inline float GetAmp() { return amp_; }
    /** Returns the current value for freq.
    */
    inline float GetFreq() { return freq_; }
    /** Returns the current value for decay.
    */
    inline float GetDecay() { return decay_; }
    /** Returns the current value for damp.
    */
    inline float GetDamp() { return damp_; }
    /** Returns the current value for mode.
    */
    inline int32_t GetMode() { return mode_; }

  private:
    void    Reinit();
    float   amp_, freq_, decay_, damp_, ifreq_;
    float   sicps_;
    int32_t phs256_, npts_, maxpts_;
    float * buf_;
    float   sample_rate_;
    char    init_;
    int32_t mode_;
};
} // namespace daisysp
#endif
#endif
