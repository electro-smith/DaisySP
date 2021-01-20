#include "modules/dsp.h"
#include "modules/chorus.h"
#include <math.h>

using namespace daisysp;

void Chorus::Init(float sample_rate){
	sample_rate_ = sample_rate;
	
	max_del_ = .06f * sample_rate_; //60 ms worth of samples
	pre_gain_ = 1.f / NUM_DEL;
	
	const float inc = .1f / sample_rate_;
	for(int i = 0; i < NUM_DEL; i++){
		del_[i].Init();
		
		lfo_phase_[i] = i * pre_gain_; //start out of phase
		lfo_freq_[i] = inc + inc * i; //different freqs
		lfo_amp_[i] = 1.f;
	}
}

float Chorus::ProcessLfo(int idx){
	lfo_phase_[idx] += lfo_freq_[idx];

	if(lfo_phase_[idx] >= 1.f){
		lfo_phase_[idx] = 1.f - (lfo_phase_[idx] - 1.f);
		lfo_freq_[idx] *= -1;
	}
	else if(lfo_phase_[idx] <= 1.f){
		lfo_phase_[idx] = -1.f * lfo_phase_[idx];
		lfo_freq_[idx] *= -1;
	}
	
	return lfo_phase_[idx] * lfo_amp_[idx];
}

float Chorus::Process(float in){
	float ret = 0.f;

	for(int i = 0; i < NUM_DEL; i++){
		float lfo_sig = ProcessLfo(i);
		del_[i].SetDelay(lfo_sig * max_del_);
		del_[i].Write(in);
		
		ret += del_[i].Read();
	}
	
	return ret * pre_gain_;
}