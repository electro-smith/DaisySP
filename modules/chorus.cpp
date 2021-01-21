#include "modules/dsp.h"
#include "modules/chorus.h"
#include <math.h>

using namespace daisysp;

void Chorus::Init(float sample_rate){
	sample_rate_ = sample_rate;
	
	pre_gain_ = 1.f / (NUM_DEL + 1);
	
	for(int i = 0; i < NUM_DEL; i++){
		del_[i].Init();
		lfo_amp_[i] = 0.f;
		SetDelay(2 + 2 * i, i);
		
		lfo_phase_[i] = 0.f;
		SetLfoFreq(.5 + .05 * i, i);
		SetLfoDepth(2, i);
	}
}

float Chorus::ProcessLfo(int idx){
	lfo_phase_[idx] += lfo_freq_[idx];

	//wrap around and flip direction
	if(lfo_phase_[idx] > 1.f){
		lfo_phase_[idx] = 1.f - (lfo_phase_[idx] - 1.f);
		lfo_freq_[idx] *= -1.f;
	}	
	else if(lfo_phase_[idx] < -1.f){
		lfo_phase_[idx] = -1.f - (lfo_phase_[idx] + 1.f);
		lfo_freq_[idx] *= -1.f;
	}
	
	return lfo_phase_[idx] * lfo_amp_[idx];
}

float Chorus::Process(float in){
	float ret = 0.f;

	for(int i = 0; i < NUM_DEL; i++){
		float lfo_sig = ProcessLfo(i);
		del_[i].SetDelay(lfo_sig + delay_[i]);
		
		ret += del_[i].Read();
		del_[i].Write(in);
	}
	
	return in * pre_gain_ +  ret * pre_gain_;
}