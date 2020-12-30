#include "dsp.h"
#include "stringsynth.h"
#include <math.h>

using namespace daisysp;

inline void StringSynthOscillator::Init()
{
    phase_       = 0.0f;
    next_sample_ = 0.0f;
    segment_     = 0.0f;

    frequency_  = 0.001f;
    saw_8_gain_ = 0.0f;
    saw_4_gain_ = 0.0f;
    saw_2_gain_ = 0.0f;
    saw_1_gain_ = 0.0f;
}

float StringSynthOscillator::Process(float        frequency,
                                     const float* unshifted_registration,
                                     float        gain)
{
    frequency *= 8.0f;

    // Deal with very high frequencies by shifting everything 1 or 2 octave
    // down: Instead of playing the 1st harmonic of a 8kHz wave, we play the
    // second harmonic of a 4kHz wave.
    size_t shift = 0;
    while(frequency > 0.5f)
    {
        shift += 2;
        frequency *= 0.5f;
    }
    // Frequency is just too high.
    if(shift >= 8)
    {
        return 0.f;
    }

    float registration[7];
	for(int i = 0; i < 7; i++){
		registration[i] = 0.f;
	}

	for (size_t i = 0; i < 7 - shift; i++){
		registration[i + shift] = unshifted_registration[i];
	}

	frequency_ = frequency;
	const float saw_8_gain = saw_8_gain_ = (registration[0] + 2.0f * registration[1]) * gain;
	const float saw_4_gain = saw_4_gain_ = (registration[2] - registration[1] + 2.0f * registration[3]) * gain;
	const float saw_2_gain = saw_2_gain_ = (registration[4] - registration[3] + 2.0f * registration[5]) * gain;
	const float saw_1_gain = saw_1_gain_ = (registration[6] - registration[5]) * gain;

    float phase       = phase_;
    float next_sample = next_sample_;
    int   segment     = segment_;

	//while
	float this_sample = next_sample;
	next_sample       = 0.0f;

	phase += frequency;
	int next_segment = static_cast<int>(phase);
	if(next_segment != segment)
	{
		float discontinuity = 0.0f;
		if(next_segment == 8)
		{
			phase -= 8.0f;
			next_segment -= 8;
			discontinuity -= saw_8_gain;
		}
		if((next_segment & 3) == 0)
		{
			discontinuity -= saw_4_gain;
		}
		if((next_segment & 1) == 0)
		{
			discontinuity -= saw_2_gain;
		}
		discontinuity -= saw_1_gain;
		if(discontinuity != 0.0f)
		{
			float fraction = phase - static_cast<float>(next_segment);
			float t        = fraction / frequency;
			this_sample += ThisBlepSample(t) * discontinuity;
			next_sample += NextBlepSample(t) * discontinuity;
		}
	}
	segment = next_segment;

	next_sample += (phase - 4.0f) * saw_8_gain * 0.125f;
	next_sample += (phase - float(segment & 4) - 2.0f) * saw_4_gain * 0.25f;
	next_sample += (phase - float(segment & 6) - 1.0f) * saw_2_gain * 0.5f;
	next_sample += (phase - float(segment & 7) - 0.5f) * saw_1_gain;
    //while
	
    next_sample_ = next_sample;
    phase_       = phase;
    segment_     = segment;
	
	return 2.0f * this_sample;
}

inline float StringSynthOscillator::ThisBlepSample(float t)
{
    return 0.5f * t * t;
}

inline float StringSynthOscillator::NextBlepSample(float t)
{
    t = 1.0f - t;
    return -0.5f * t * t;
}
