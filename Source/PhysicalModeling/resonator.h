/*
Copyright (c) 2020 Electrosmith, Corp, Emilie Gillet

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/

#pragma once
#ifndef DSY_RESONATOR_H
#define DSY_RESONATOR_H

#include <stdint.h>
#include <stddef.h>
#include "Utility/dsp.h"
#ifdef __cplusplus


/** @file resonator.h */

namespace daisysp
{
// We render 4 modes simultaneously since there are enough registers to hold
// all state variables.
/**  
       @brief SVF for use in the Resonator Class \n 
       @author Ported by Ben Sergentanis 
       @date Jan 2021 
       Ported from pichenettes/eurorack/plaits/dsp/physical_modelling/resonator.h \n
       to an independent module. \n
       Original code written by Emilie Gillet in 2016. \n
*/
template <int batch_size>
class ResonatorSvf
{
  public:
    enum FilterMode
    {
        LOW_PASS,
        BAND_PASS,
        BAND_PASS_NORMALIZED,
        HIGH_PASS
    };

    ResonatorSvf() {}
    ~ResonatorSvf() {}

    void Init()
    {
        for(int i = 0; i < batch_size; ++i)
        {
            state_1_[i] = state_2_[i] = 0.0f;
        }
    }

    template <FilterMode mode, bool add>
    void Process(const float* f,
                 const float* q,
                 const float* gain,
                 const float  in,
                 float*       out)
    {
        float g[batch_size];
        float r[batch_size];
        float r_plus_g[batch_size];
        float h[batch_size];
        float state_1[batch_size];
        float state_2[batch_size];
        float gains[batch_size];
        for(int i = 0; i < batch_size; ++i)
        {
            g[i]        = fasttan(f[i]);
            r[i]        = 1.0f / q[i];
            h[i]        = 1.0f / (1.0f + r[i] * g[i] + g[i] * g[i]);
            r_plus_g[i] = r[i] + g[i];
            state_1[i]  = state_1_[i];
            state_2[i]  = state_2_[i];
            gains[i]    = gain[i];
        }

        float s_in  = in;
        float s_out = 0.0f;
        for(int i = 0; i < batch_size; ++i)
        {
            const float hp
                = (s_in - r_plus_g[i] * state_1[i] - state_2[i]) * h[i];
            const float bp = g[i] * hp + state_1[i];
            state_1[i]     = g[i] * hp + bp;
            const float lp = g[i] * bp + state_2[i];
            state_2[i]     = g[i] * bp + lp;
            s_out += gains[i] * ((mode == LOW_PASS) ? lp : bp);
        }
        if(add)
        {
            *out++ += s_out;
        }
        else
        {
            *out++ = s_out;
        }

        for(int i = 0; i < batch_size; ++i)
        {
            state_1_[i] = state_1[i];
            state_2_[i] = state_2[i];
        }
    }

  private:
    static constexpr float kPiPow3 = PI_F * PI_F * PI_F;
    static constexpr float kPiPow5 = kPiPow3 * PI_F * PI_F;
    static inline float    fasttan(float f)
    {
        const float a  = 3.260e-01 * kPiPow3;
        const float b  = 1.823e-01 * kPiPow5;
        float       f2 = f * f;
        return f * (PI_F + f2 * (a + b * f2));
    }

    float state_1_[batch_size];
    float state_2_[batch_size];
};


/**         
       @brief Resonant Body Simulation
       @author Ported by Ben Sergentanis 
       @date Jan 2021 
       Ported from pichenettes/eurorack/plaits/dsp/physical_modelling/resonator.h \n
       to an independent module. \n
       Original code written by Emilie Gillet in 2016. \n 
*/
class Resonator
{
  public:
    Resonator() {}
    ~Resonator() {}

    /** Initialize the module
        \param position    Offset the phase of the amplitudes. 0-1
        \param resolution Quality vs speed scalar
        \param sample_rate Samplerate of the audio engine being run.
    */
    void Init(float position, int resolution, float sample_rate);

    /** Get the next sample_rate
        \param in The signal to excited the resonant body
    */
    float Process(const float in);

    /** Resonator frequency.
        \param freq Frequency in Hz.
    */
    void SetFreq(float freq);

    /** Changes the general charater of the resonator (stiffness, brightness)
        \param structure Works best from 0-1
    */
    void SetStructure(float structure);

    /** Set the brighness of the resonator
        \param brightness Works best 0-1
    */
    void SetBrightness(float brightness);

    /** How long the resonant body takes to decay.
        \param damping Works best 0-1
    */
    void SetDamping(float damping);

  private:
    int   resolution_;
    float frequency_, brightness_, structure_, damping_;

    static constexpr int   kMaxNumModes   = 24;
    static constexpr int   kModeBatchSize = 4;
    static constexpr float ratiofrac_     = 1.f / 12.f;
    static constexpr float stiff_frac_    = 1.f / 64.f;
    static constexpr float stiff_frac_2   = 1.f / .6f;

    float sample_rate_;

    float CalcStiff(float sig);

    float                        mode_amplitude_[kMaxNumModes];
    ResonatorSvf<kModeBatchSize> mode_filters_[kMaxNumModes / kModeBatchSize];
};

} // namespace daisysp
#endif
#endif
