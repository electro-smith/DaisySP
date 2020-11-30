#include "fm2.h"

using namespace daisysp;

void Fm2::Init(float samplerate)
{
  //init oscillators
  car_.Init(samplerate);
  mod_.Init(samplerate);

  //set some reasonable values
  car_.SetFreq(440.f);
  mod_.SetFreq(220.f);

  car_.SetAmp(1.f);
  mod_.SetAmp(1.f);

  car_.SetWaveform(Oscillator::WAVE_SIN);
  mod_.SetWaveform(Oscillator::WAVE_SIN);
  
  idx_ = 30.f;  
}

float Fm2::Process()
{
  float modval = mod_.Process();
  car_.SetFreq(modval * idx_ + car_freq_);
  return car_.Process();
}
	
void Fm2::SetCarrierFrequency(float freq)
{
  car_.SetFreq(freq);
}

void Fm2::SetModulatorFrequency(float freq)
{
  mod_.SetFreq(freq);
}
		
void Fm2::SetRatio(float ratio)
{
  ratio = fabsf(ratio);
  mod_.SetFreq(ratio * car_freq_);
}
	
void Fm2::SetCarrierWaveform(uint8_t wave)
{
  car_.SetWaveform(wave);
}

void Fm2::SetModulatorWaveform(uint8_t wave)
{
  mod_.SetWaveform(wave);
}

void Fm2::SetIndex(float index)
{
  idx_ = fabsf(index);
}

float Fm2::GetIndex()
{
  return idx_;
}

void Fm2::Reset()
{
  car_.Reset();
  mod_.Reset();
}
