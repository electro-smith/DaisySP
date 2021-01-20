#include "modules/dsp.h"
#include "modules/chorus.h"
#include <math.h>

using namespace daisysp;

void Chorus::Init(float sample_rate){
	sample_rate_ = sample_rate;
	
	pre_gain_ = 1.f / (NUM_DEL + 1);
	
	const float inc = .1f / sample_rate_;
	for(int i = 0; i < NUM_DEL; i++){
		del_[i].Init();
		delay_[i] = .01f * i * sample_rate_; //10 ms, 20ms, ...
		
		lfo_phase_[i] = i * pre_gain_; //start out of phase
		lfo_freq_[i] = inc + inc * i; //different freqs
		lfo_amp_[i] = .015 * sample_rate_; // +/- 15ms
	}
}

float Chorus::ProcessLfo(int idx){
	lfo_phase_[idx] += lfo_freq_[idx];

	//wrap around and flip direction
	if(lfo_phase_[idx] >= 1.f || lfo_phase_[idx] <= -1.f){
		float one = lfo_phase_[idx] >= 1.f ? -1.f : 1.f;
		lfo_phase_[idx] = (one * -1.f) - (lfo_phase_[idx] + one);
		lfo_freq_[idx] *= -1;
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