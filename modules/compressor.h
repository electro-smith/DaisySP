#pragma once
#ifndef DSY_COMPRESSOR_H
#define DSY_COMPRESSOR_H

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

by: shensley, improved upon by AvAars
\todo Still pretty expensive
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

    /** compress the audio input signal
	    \param in audio input signal
    */
    float Process(float in);

    /** compress the audio signal, based on the calculated gain
    */
    inline float GetGain(float in = 1.0f) { return gain_ * in; }

    /** compresses the audio input signal, keyed by a secondary input.
        \param in audio input signal (to be compressed)
        \param key audio input that will be used to side-chain the compressor. 
    */
    inline float Process(float in, float key)
    {
        Process(key);
        return GetGain(in);
    }

    inline void ProcessBlock(float *in, float *out, size_t size)
    {
        ProcessBlock(in, out, in, size);
    }

    void ProcessBlock(float *in, float *out, float *key, size_t size);

    void ProcessBlock(float **in,
                      float **out,
                      float * key,
                      size_t  channels,
                      size_t  size);


    /** Sets the amount of gain reduction applied to compressed signals
        \param ratio Expects 1.0 -> 40. (untested with values < 1.0)
    */
    inline void SetRatio(float ratio)
    {
        ratio_ = ratio;
        RecalculateRatio();
    }

    /** Sets the threshold in dB at which compression will be applied
        \param threshold Expects 0.0 -> -80.
    */
    inline void SetThreshold(float threshold)
    {
        thresh_ = threshold;
        RecalculateMakeup();
    }

    /** Sets the envelope time for onset of compression for signals above the threshold.
        \param attack Expects 0.001 -> 10
    */
    inline void SetAttack(float attack)
    {
        atk_ = attack;
        RecalculateAttack();
    }

    /** Sets the envelope time for release of compression as input signal falls below threshold.
        \param release Expects 0.001 -> 10
    */
    inline void SetRelease(float release)
    {
        rel_ = release;
        RecalculateRelease();
    }

    /** Manually sets the additional gain to make up for the compression
        \param gain Expects 0.0 -> 80
    */
    inline void SetMakeup(float gain) { makeup_gain_ = gain; }

    /** Enables or disables the automatic makeup gain. Disabling sets the makeup gain to 0.0
        \param enable enable or disable
    */
    inline void AutoMakeup(bool enable)
    {
        makeup_auto_ = enable;
        makeup_gain_ = 0.0f;
        RecalculateMakeup();
    }

  private:
    float ratio_, thresh_, atk_, rel_;
    float makeup_gain_;
    float gain_;
    /** internals from faust struct
*/
    float slope_rec_[2], gain_rec_[2];
    float atk_slo2_, ratio_mul_, atk_slo_, rel_slo_;
    int   sample_rate_;
    float sample_rate_inv2, sample_rate_inv_;

    bool makeup_auto_;

    inline void RecalculateRatio()
    {
        ratio_mul_ = ((1.0f - atk_slo2_) * ((1.0f / ratio_) - 1.0f));
    }

    inline void RecalculateAttack()
    {
        atk_slo_  = expf((-(sample_rate_inv_ / atk_)));
        atk_slo2_ = expf(-(sample_rate_inv2 / atk_));

        RecalculateRatio();
    }

    inline void RecalculateRelease()
    {
        rel_slo_ = expf((-(sample_rate_inv_ / rel_)));
    }

    inline void RecalculateMakeup()
    {
        if(makeup_auto_)
            makeup_gain_ = fabsf(thresh_ - thresh_ / ratio_) * 0.5f;
    }
};

} // namespace daisysp

#endif // DSY_COMPRESSOR_H
