#pragma once
#ifndef DSY_WAVETABLEOSC_H
#define DSY_WAVETABLEOSC_H

#include "Utility/wavetables.h"


namespace daisysp
{
/**
 * @brief Wave table oscillator
 *
 * Based on implementation from STK (by Perry R. Cook and Gary P. Scavone, 1995--2021)
 */
class WavetableOsc
{
  public:
    enum Waveform
    {
        WAVE_SIN,
        WAVE_TRI,
        WAVE_SAW,
        WAVE_SQUARE,
        WAVE_LAST,
    };

    void Init(float sample_rate_)
    {
        sample_rate   = sample_rate_;
        sr_resiprocal = 1 / sample_rate;
        SetWaveform(WAVE_SQUARE);
        SetFreq(440);
        SetAmp(0.7f);
    }

    void SetWaveform(const Waveform wf)
    {
        switch(wf)
        {
            case Waveform::WAVE_SIN: SetWaveTable(&Tables::Sine); break;
            case Waveform::WAVE_TRI: SetWaveTable(&Tables::Tri); break;
            case Waveform::WAVE_SAW: SetWaveTable(&Tables::Saw); break;
            case Waveform::WAVE_SQUARE: SetWaveTable(&Tables::Square); break;
            default: SetWaveTable(&Tables::Sine);
        }
    }

    void SetWaveTable(WaveTable *table)
    {
        m_table    = table;
        table_size = m_table->buff->wt_size;
    }

    /**
     * @brief  Set the data interpolation rate based on a looping frequency.
     * 
     * This function determines the interpolation rate based on the file
     * size and the current Stk::sampleRate.  The \e frequency value
     * corresponds to file cycles per second.  The frequency can be
     * negative, in which case the loop is read in reverse order.
     * 
     * @param frequency Osc frequency in Hz.
     */
    void SetFreq(float frequency)
    {
        auto norm_freq = frequency * sr_resiprocal;
        this->SetRate(table_size * norm_freq);

        // update the current wave table selector
        m_table->SetTopFreq(norm_freq);
    }

    /**
     * @brief Add a phase offset in cycles, where 1.0 = fileSize.
     * 
     * 
     * This function determines a time offset based on the file
     * size and the current sample sate.  The \e angle value
     * is a multiple of file size.
     * 
     * @param angle Phase offset in cycles.
     */
    void PhaseAdd(float angle)
    {
        time_ += table_size * angle;

        if(time_ < 0.0f)
            time_ += table_size;
        else if(time_ >= table_size)
            time_ -= table_size;
    }

    void SetAmp(const float a) { amp = a; }

    /**
     * @brief Compute a sample frame and return the value.
     */
    float Process()
    {
        while(time_ < 0.0f)
            time_ += table_size;
        while(time_ >= table_size)
            time_ -= table_size;

        float out = m_table->GetSample(time_);

        time_ += rate_;

        return out * amp;
    }

  private:
    /**
    * @brief Set the data read rate in samples.  The rate can be negative.
    * 
    * If the rate value is negative, the data is read in reverse order.
    * 
    * @param rate 
    */
    void SetRate(float rate)
    {
        rate_ = rate;

        if(std::fmod(rate_, 1.0f) != 0.0f)
            interpolate_ = true;
        else
            interpolate_ = false;
    }

    WaveTable *m_table;
    float      sample_rate;
    float      sr_resiprocal;
    float      time_        = 0.f;
    bool       interpolate_ = false;
    float      rate_;
    float      amp        = 1;
    uint16_t   table_size = 1;
};

} // namespace daisysp
#endif
