/*
Copyright (c) 2020 Electrosmith, Corp, Emilie Gillet

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/

#ifndef DSY_ONEPOLE_H
#define DSY_ONEPOLE_H

#include "Utility/dsp.h"
#include <cmath>
#include <algorithm>

namespace daisysp
{
/**         
    @brief One Pole Lowpass / Highpass Filter
    @author beserge
    @date Dec 2023 
    Ported from pichenettes/stmlib/dsp/filter.h \n 
    \n to an independent module. \n
    Original code written by Emilie Gillet. \n
*/
class OnePole
{
  public:
    OnePole() {}
    ~OnePole() {}

    /**
     * @brief Operational modes of the filter
     * - LOW_PASS = cuts out high frequency sound, lets low frequency sound pass through
     * - HIGH_PASS = cuts out low frequency sound, lets high frequency sound pass through
     */
    enum FilterMode
    {
        FILTER_MODE_LOW_PASS,
        FILTER_MODE_HIGH_PASS
    };

    /** Initializes the module */
    void Init()
    {
        Reset();
        mode_ = FILTER_MODE_LOW_PASS;
    }

    /** Reset the module to its default state */
    inline void Reset() { state_ = 0.0f; }

    /** Set the filter cutoff frequency
    *   \param freq Cutoff frequency. Valid range from 0 to .497f
    */
    inline void SetFrequency(float freq)
    {
        // Clip coefficient to about 100.
        freq = freq < 0.497f ? freq : 0.497f;

        g_  = tanf(PI_F * freq);
        gi_ = 1.f / (1.f + g_);
    }

    /** Set the filter mode
    *   \param mode Filter mode. Can be lowpass or highpass
    */
    inline void SetFilterMode(FilterMode mode) { mode_ = mode; }

    /** Process audio through the filter
    *   \param in The next sample to be processed
    */
    inline float Process(float in)
    {
        float lp;
        lp     = (g_ * in + state_) * gi_;
        state_ = g_ * (in - lp) + lp;

        switch(mode_)
        {
            case FILTER_MODE_LOW_PASS: return lp;
            case FILTER_MODE_HIGH_PASS: return in - lp;
        }

        return 0.0f;
    }

    /** Process a block of audio through the filter
    *   \param in_out Pointer to the block of samples to be processed
    *   \param size Size of the block of samples to be processed.
    */
    inline void ProcessBlock(float* in_out, size_t size)
    {
        while(size--)
        {
            *in_out = Process(*in_out);
            ++in_out;
        }
    }

  private:
    float      g_;
    float      gi_;
    float      state_;
    FilterMode mode_;
};

} // namespace daisysp

#endif // DSY_ONEPOLE_H
