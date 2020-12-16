#include "fm2.h"

using namespace daisysp;

void Fm2::Init(float samplerate)
{
    //init oscillators
    car_.Init(samplerate);
    mod_.Init(samplerate);

    //set some reasonable values
    SetFrequency(440.f);
    SetRatio(2.f);
    lfreq_  = 440.f;
    lratio_ = 2.f;

    car_.SetAmp(1.f);
    mod_.SetAmp(1.f);

    car_.SetWaveform(Oscillator::WAVE_SIN);
    mod_.SetWaveform(Oscillator::WAVE_SIN);

    idx_       = 1.f;
    idxScalar_ = .2f;
}

float Fm2::Process()
{
    if(lratio_ != ratio_ || lfreq_ != freq_)
    {
        lratio_ = ratio_;
        lfreq_  = freq_;
        car_.SetFreq(freq_);
        mod_.SetFreq(freq_ * ratio_);
    }

    float modval = mod_.Process();
    car_.PhaseAdd(modval * idx_);
    return car_.Process();
}

void Fm2::SetFrequency(float freq)
{
    freq_ = fabsf(freq);
}

void Fm2::SetRatio(float ratio)
{
    ratio_ = fabsf(ratio);
}

void Fm2::SetIndex(float index)
{
    idx_ = index * idxScalar_;
}

float Fm2::GetIndex()
{
    return idx_ / idxScalar_;
}

void Fm2::Reset()
{
    car_.Reset();
    mod_.Reset();
}
