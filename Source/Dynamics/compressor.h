#pragma once
#ifndef DSY_COMPRESSOR_H
#define DSY_COMPRESSOR_H

#include "Utility/dsp.h"

namespace daisysp
{
/** dynamics compressor

influenced by compressor in soundpipe (from faust).

Modifications made to do:
- Less calculations during each process loop (coefficients recalculated on parameter change).
- C++-ified
- added sidechain support
- pulled gain apart for monitoring and multichannel support
- improved readability
- improved makeup-gain calculations
- changing controls now costs a lot less
- a lot less expensive

by: shensley, improved upon by AvAars
\todo Add soft/hard knee settings
*/
class Compressor
{
  public:
    Compressor() {}
    ~Compressor() {}
    /** Initializes compressor
        \param sample_rate rate at which samples will be produced by the audio engine.
    */
    void Init(float sample_rate);

    /** Compress the audio input signal, saves the calculated gain
        \param in audio input signal
    */
    float Process(float in);

    /** Compresses the audio input signal, keyed by a secondary input.
        \param in audio input signal (to be compressed)
        \param key audio input that will be used to side-chain the compressor
    */
    float Process(float in, float key)
    {
        Process(key);
        return Apply(in);
    }

    /** Apply compression to the audio signal, based on the previously calculated gain
        \param in audio input signal
    */
    float Apply(float in) { return gain_ * in; }

    /** Compresses a block of audio
        \param in audio input signal
        \param out audio output signal
        \param size the size of the block
    */
    void ProcessBlock(float *in, float *out, size_t size)
    {
        ProcessBlock(in, out, in, size);
    }

    /** Compresses a block of audio, keyed by a secondary input
        \param in audio input signal (to be compressed)
        \param out audio output signal
        \param key audio input that will be used to side-chain the compressor
        \param size the size of the block
    */
    void ProcessBlock(float *in, float *out, float *key, size_t size);

    /** Compresses a block of multiple channels of audio, keyed by a secondary input
        \param in audio input signals (to be compressed)
        \param out audio output signals
        \param key audio input that will be used to side-chain the compressor
        \param channels the number of audio channels
        \param size the size of the block
    */
    void ProcessBlock(float **in,
                      float **out,
                      float * key,
                      size_t  channels,
                      size_t  size);

    /** Gets the amount of gain reduction */
    float GetRatio() { return ratio_; }

    /** Sets the amount of gain reduction applied to compressed signals
     \param ratio Expects 1.0 -> 40. (untested with values < 1.0)
    */
    void SetRatio(float ratio)
    {
        ratio_ = ratio;
        RecalculateRatio();
    }

    /** Gets the threshold in dB */
    float GetThreshold() { return thresh_; }

    /** Sets the threshold in dB at which compression will be applied
     \param threshold Expects 0.0 -> -80.
    */
    void SetThreshold(float threshold)
    {
        thresh_ = threshold;
        RecalculateMakeup();
    }

    /** Gets the envelope time for onset of compression */
    float GetAttack() { return atk_; }

    /** Sets the envelope time for onset of compression for signals above the threshold.
        \param attack Expects 0.001 -> 10
    */
    void SetAttack(float attack)
    {
        atk_ = attack;
        RecalculateAttack();
    }

    /** Gets the envelope time for release of compression */
    float GetRelease() { return rel_; }

    /** Sets the envelope time for release of compression as input signal falls below threshold.
        \param release Expects 0.001 -> 10
    */
    void SetRelease(float release)
    {
        rel_ = release;
        RecalculateRelease();
    }

    /** Gets the additional gain to make up for the compression */
    float GetMakeup() { return makeup_gain_; }

    /** Manually sets the additional gain to make up for the compression
        \param gain Expects 0.0 -> 80
    */
    void SetMakeup(float gain) { makeup_gain_ = gain; }

    /** Enables or disables the automatic makeup gain. Disabling sets the makeup gain to 0.0
        \param enable true to enable, false to disable
    */
    void AutoMakeup(bool enable)
    {
        makeup_auto_ = enable;
        makeup_gain_ = 0.0f;
        RecalculateMakeup();
    }

    /** Gets the gain reduction in dB
    */
    float GetGain() { return fastlog10f(gain_) * 20.0f; }

  private:
    float ratio_, thresh_, atk_, rel_;
    float makeup_gain_;
    float gain_;

    // Recorded slope and gain, used in next sample
    float slope_rec_, gain_rec_;

    // Internals from faust
    float atk_slo2_, ratio_mul_, atk_slo_, rel_slo_;

    int   sample_rate_;
    float sample_rate_inv2_, sample_rate_inv_;

    // Auto makeup gain enable
    bool makeup_auto_;

    // Methods for recalculating internals
    void RecalculateRatio()
    {
        ratio_mul_ = ((1.0f - atk_slo2_) * ((1.0f / ratio_) - 1.0f));
    }

    void RecalculateAttack()
    {
        atk_slo_  = expf(-(sample_rate_inv_ / atk_));
        atk_slo2_ = expf(-(sample_rate_inv2_ / atk_));

        RecalculateRatio();
    }

    void RecalculateRelease() { rel_slo_ = expf((-(sample_rate_inv_ / rel_))); }

    void RecalculateMakeup()
    {
        if(makeup_auto_)
            makeup_gain_ = fabsf(thresh_ - thresh_ / ratio_) * 0.5f;
    }
};

} // namespace daisysp

#endif // DSY_COMPRESSOR_H
