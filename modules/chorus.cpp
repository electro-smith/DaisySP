#include "dsp.h"
#include "chorus.h"
#include <math.h>

using namespace daisysp;

void Chorus::Init(float sample_rate){
	sample_rate_ = sample_rate;
	
}

float Chorus::Process(float in){
	return 0.f;
}