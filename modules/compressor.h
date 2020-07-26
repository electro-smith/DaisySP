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

by: shensley, improved upon by AvAars

\todo With fixed controls this is relatively quick, but changing controls now costs a lot more
\todo Still pretty expensive
\todo Add soft/hard knee settings
\todo Calculate only the necessary internals on parameter changes
*/
class Compressor
{
  public:
    Compressor() {}
    ~Compressor() {}
    /** Initializes compressor
        sample_rate - rate at which samples will be produced by the audio engine.
    */
    void Init(float sample_rate);

    /** calculate the gain of the compressor, based on the key 
    */
    float Process(float in);

    /** compress the audio signal, based on the calculated gain
    */
    inline float GetGain(float in = 1.0f) { return gain_ * in; }

    /** compresses the audio input signal, keyed by a secondary input.
        \param in - audio input signal (to be compressed)
        \param key - audio input that will be used to side-chain the compressor. 
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


    /** amount of gain reduction applied to compressed signals
        Expects 1.0 -> 40. (untested with values < 1.0)
    */
    inline void SetRatio(float ratio)
    {
        ratio_ = ratio;
        RecalculateSlopes();
    }

    /** threshold in dB at which compression will be applied
        Expects 0.0 -> -80.
    */
    inline void SetThreshold(float thresh)
    {
        thresh_ = thresh;
        RecalculateSlopes();
    }

    /** envelope time for onset of compression for signals above the threshold.
        Expects 0.001 -> 10
    */
    inline void SetAttack(float atk)
    {
        atk_ = atk;
        RecalculateSlopes();
    }

    /** envelope time for release of compression as input signal falls below threshold.
        Expects 0.001 -> 10
    */
    inline void SetRelease(float rel)
    {
        rel_ = rel;
        RecalculateSlopes();
    }

    inline void ApplyMakeup(bool makeup)
    {
        makeup_mul_ = makeup ? 1.0f : 0.0f;
        RecalculateSlopes();
    }

  private:
    void  RecalculateSlopes();
    float ratio_, thresh_, atk_, rel_;
    float makeup_gain_, makeup_mul_;
    float gain_;
    /** internals from faust struct
*/
    float slope_rec_[2], f_rec1_[2], gain_rec_[2];
    float atk_slo2_, ratio_mul_, atk_slo_, rel_slo_;
    int   sample_rate_;
    float f_const0_, sample_rate_inv2, sample_rate_inv_;
};

} // namespace daisysp

#endif // DSY_COMPRESSOR_H
