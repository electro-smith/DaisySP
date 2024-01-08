/*
Copyright (c) 2020 Electrosmith, Corp, Vinícius Fernandes

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/

#pragma once
#ifndef DSY_GRANULARPLAYER_H
#define DSY_GRANULARPLAYER_H

#include <stdint.h>
#include <cmath>
#include "Control/phasor.h"
#ifdef __cplusplus
#ifndef M_PI
#define M_PI 3.14159265358979323846 /* pi */
#endif

namespace daisysp
{
/** GranularPlayer Module

    Date: November, 2023

    Author: Vinícius Fernandes

    GranularPlayer is a lookup table player that provides independent time stretching and pitch shifting 
    via granulation.
    Inspired by the grain.player object from else pure data's library.
*/

class GranularPlayer
{
  public:
    GranularPlayer() {}
    ~GranularPlayer() {}

    /** Initializes the GranularPlayer module.
        \param sample pointer to the sample to be played
        \param size number of elements in the sample array
        \param sample_rate audio engine sample rate
    */
    void Init(float* sample, int size, float sample_rate);

    /** Processes the granular player.
        \param speed playback speed. 1 is normal speed, 2 is double speed, 0.5 is half speed, etc. Negative values play the sample backwards.
        \param transposition transposition in cents. 100 cents is one semitone. Negative values transpose down, positive values transpose up.
        \param grain_size grain size in milliseconds. 1 is 1 millisecond, 1000 is 1 second. Does not accept negative values. Minimum value is 1.
    */
    float Process(float speed, float transposition, float grain_size);

  private:
    //Wraps an index to the size of the sample array
    uint32_t WrapIdx(uint32_t idx, uint32_t size);

    //Converts cents(1/100th of a semitone) to a ratio
    float CentsToRatio(float cents);

    //Converts milliseconds to  number of samples
    float MsToSamps(float ms, float samplerate);

    //Inverts the phase of the phasor if the frequency is negative, mimicking pure data's phasor~ object
    float NegativeInvert(Phasor* phs, float frequency);


    float* sample_;        //pointer to the sample to be played
    float  sample_rate_;   //audio engine sample rate
    int    size_;          //number of elements in the sample array
    float  grain_size_;    //grain size in milliseconds
    float  speed_;         //processed playback speed.
    float  transposition_; //processed transpotion.
    float  sample_frequency_;
    float  cosEnv_[256] = {0}; //cosine envelope for crossfading between grains
    float
        idxTransp_; // Adjusted Transposition value contribution to idx of first grain
    float
          idxTransp2_; // Adjusted Transposition value contribution to idx of second grain
    float idxSpeed_; // Adjusted Speed value contribution to idx of first grain
    float
          idxSpeed2_; // Adjusted Speed value contribution to idx of second grain
    float sig_;       // Output of first grain
    float sig2_;      // Output of second grain

    uint32_t idx_;  // Index of first grain
    uint32_t idx2_; // Index of second grain

    Phasor phs_;     // Phasor for speed
    Phasor phsImp_;  // Phasor for transposition
    Phasor phs2_;    // Phasor for speed
    Phasor phsImp2_; // Phasor for transposition
};
} // namespace daisysp
#endif
#endif