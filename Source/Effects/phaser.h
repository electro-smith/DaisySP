#pragma once
#ifndef DSY_PHASER_H
#define DSY_PHASER_H
#ifdef __cplusplus

#include <stdint.h>
#include "Utility/delayline.h"
#include "Utility/dsp.h"

//#define MAX_POLES 8

/** @file phaser.h */

namespace daisysp
{
/**  
    @brief Single Phaser engine. Used in Phaser.
	@author Ben Sergentanis
*/
class PhaserEngine
{
  public:
    PhaserEngine() {}
    ~PhaserEngine() {}

    /** Initialize the module
		\param sample_rate Audio engine sample rate.
	*/
    void Init(float sample_rate);

    /** Get the next sample
		\param in Sample to process
	*/
    float Process(float in);

    /** How much to modulate the allpass filter by.
		\param depth Works 0-1.
	*/
    void SetLfoDepth(float depth);

    /** Set lfo frequency.
		\param lfo_freq Frequency in Hz
	*/
    void SetLfoFreq(float lfo_freq);

    /** Set the allpass frequency
		\param ap_freq Frequency in Hz.
	*/
    void SetFreq(float ap_freq);

    /** Set the feedback amount.
		\param feedback Amount from 0-1.
	*/
    void SetFeedback(float feedback);

  private:
    static constexpr int32_t kDelayLength
        = 2400; // 50 ms at 48kHz = .05 * 48000

    float sample_rate_;
    //triangle lfo
    float lfo_phase_;
    float lfo_freq_;
    float lfo_amp_;

    float os_;

    float feedback_;
    float ap_freq_;

    float deltime_;
    float last_sample_;

    DelayLine<float, kDelayLength> del_;

    float ProcessLfo();
};

class AllPassFilter
{
  public:
    AllPassFilter() {}
    ~AllPassFilter() {}

    inline void Init(float samplerate)
    {
        sr_    = samplerate;
        isr_   = 1.f / sr_;
        coeff_ = 1.f;
        b_     = 0.f;
    }

    inline void SetFreq(float freq)
    {
        if(lfreq_ != freq)
        {
            lfreq_ = freq;
            // Recompute coeff
            float theta, d;
            theta  = PI_F * freq * isr_;
            d      = tan(theta);
            coeff_ = fclamp((1.f - d) / (1.f + d), -1.f, 1.f);
        }
    }

    inline float Process(float in)
    {
        last_ = in + coeff_ * last_;
        return -in * coeff_ + last_;
    }

    inline float GetCoeff() { return coeff_; }

  private:
    float sr_, isr_;
    float lfreq_, coeff_;
    float b_;
    float last_;
};

//wraps up all of the phaser engines
/**  
    @brief Phaser Effect.
	@author Ben Sergentanis
	@date Jan 2021
*/
class Phaser
{
  public:
    Phaser() {}
    ~Phaser() {}

    /** Initialize the module
		\param sample_rate Audio engine sample rate
	*/
    void Init(float sample_rate);

    /** Get the next floating point sample.
		\param in Sample to process
	*/
    float Process(float in);

    /** Number of allpass stages.
		\param poles Works 1 to 8.
	*/
    void SetPoles(int poles);

    /** Set all lfo depths
		\param depth Works 0-1.
	*/
    void SetLfoDepth(float depth);

    /** Set all lfo frequencies.
		\param lfo_freq Lfo freq in Hz.
	*/
    void SetLfoFreq(float lfo_freq);

    /** Set all channel allpass freq in Hz.
		\param ap_freq Frequency in Hz.
	*/
    void SetFreq(float ap_freq);

    /** Set all channels feedback.
		\param feedback Works 0-1.
	*/
    void SetFeedback(float feedback);

  private:
    static constexpr int kMaxPoles = 8;
    //    DelayLine<float, 2>  apf_[kMaxPoles];
    AllPassFilter apf_[kMaxPoles];
    //    PhaserEngine engines_[kMaxPoles];
    float  gain_frac_;
    float  freq_;
    float  rsr_, sr_;
    float  last_, feedback_;
    float  c_[kMaxPoles];
    float  pfreq_[kMaxPoles];
    size_t poles_;
};
} //namespace daisysp
#endif
#endif
