/*
Copyright (c) 2020 Electrosmith, Corp

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/

#pragma once
#ifndef DECIMATOR_H
#define DECIMATOR_H
#include <stdint.h>
#ifdef __cplusplus

namespace daisysp
{
/** Performs downsampling and bitcrush effects
*/
class Decimator
{
  public:
    Decimator() {}
    ~Decimator() {}
    /** Initializes downsample module
    */
    void Init();

    /** Applies downsample and bitcrush effects to input signal.
        \return one sample. This should be called once per sample period. 
    */
    float Process(float input);


    /** Sets amount of downsample 
        Input range: 
    */
    inline void SetDownsampleFactor(float downsample_factor)
    {
        downsample_factor_ = downsample_factor;
    }

    /** Sets amount of bitcrushing 
        Input range: 0...1.0
    */
    inline void SetBitcrushFactor(float bitcrush_factor)
    {
        bitcrush_factor_ = bitcrush_factor;
        bits_to_crush_   = (uint32_t)(bitcrush_factor * kMaxBitsToCrush);
        bit_overflow_
            = 2.0f - (bitcrush_factor * 16.0f) + (float)(bits_to_crush_);
    }

    /** Sets the exact number of bits to crush and disables smooth crushing
        0-16 bits
    */
    inline void SetBitsToCrush(const uint8_t &bits)
    {
        bits_to_crush_   = bits <= kMaxBitsToCrush ? bits : kMaxBitsToCrush;
        smooth_crushing_ = false;
    }

    /** Sets the smooth crushing on or off
        true/false
    */
    inline void SetSmoothCrushing(bool smooth_crushing)
    {
        smooth_crushing_ = smooth_crushing;
    }

    /** Returns current setting of smooth crushing
    */
    inline bool GetSmoothCrushing() { return smooth_crushing_; }
    /** Returns current setting of downsample
    */
    inline float GetDownsampleFactor() { return downsample_factor_; }
    /** Returns current setting of bitcrush
    */
    inline float GetBitcrushFactor() { return bitcrush_factor_; }
    /** Returns current bitcrush setting in bits
    */
    inline int GetBitsToCrush() { return bits_to_crush_; }

  private:
    const uint8_t kMaxBitsToCrush = 16;
    float         downsample_factor_, bitcrush_factor_;
    uint32_t      bits_to_crush_;
    float         downsampled_, bitcrushed_;
    uint32_t      inc_, threshold_;
    bool          smooth_crushing_;
    float         bit_overflow_;
};
} // namespace daisysp
#endif
#endif
