#include "dsp.h"
#include "sampleratereducer.h"
#include <math.h>

using namespace daisysp;
void SampleRateReducer::Init()
{
    frequency_       = .5f; //half sample rate
    phase_           = 0.0f;
    sample_          = 0.0f;
    next_sample_     = 0.0f;
    previous_sample_ = 0.0f;
}

float SampleRateReducer::Process(float in)
{
    float previous_sample = previous_sample_;
    float next_sample     = next_sample_;
    float sample          = sample_;
    float phase           = phase_;

    float this_sample = next_sample;
    next_sample       = 0.0f;
    phase += frequency_;
    if(phase >= 1.0f)
    {
        phase -= 1.0f;
        float t = phase / frequency_;
        // t = 0: the transition occurred right at this sample.
        // t = 1: the transition occurred at the previous sample.
        // Use linear interpolation to recover the fractional sample.
        float new_sample
            = previous_sample + (in - previous_sample) * (1.0f - t);
        float discontinuity = new_sample - sample;
        this_sample += discontinuity * ThisBlepSample(t);
        next_sample += discontinuity * NextBlepSample(t);
        sample = new_sample;
    }
    next_sample += sample;
    previous_sample = in;

    phase_           = phase;
    next_sample_     = next_sample;
    sample_          = sample;
    previous_sample_ = previous_sample;

    return this_sample;
}

void SampleRateReducer::SetFreq(float frequency)
{
    frequency_ = fclamp(frequency_, 0.f, 1.f);
}
