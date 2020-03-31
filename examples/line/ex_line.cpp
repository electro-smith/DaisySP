#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;
using namespace daisy;

static DaisySeed seed;
static Line line_seg;
static Oscillator osc_sine;

uint8_t finished;

static void AudioCallback(float *in, float *out, size_t size)
{
	float sine, freq;
    for (size_t i = 0; i < size; i += 2)
    {
        if (finished)
        {
            // Start creating a Line segment from 100 to 500 in 1 seconds
            line_seg.Start(100, 500, 1);
        }

        freq = line_seg.Process(&finished);
        osc_sine.SetFreq(freq);
    	sine = osc_sine.Process();

    	// left out
        out[i] = sine;

        // right out
        out[i+1] = sine;
    }
}

int main(void)
{
	// initialize seed hardware and daisysp modules
    float sample_rate;
	seed.Configure();
	seed.Init();
	sample_rate = seed.AudioSampleRate();

    // initialize Line module
    line_seg.Init(sample_rate);
    finished = 1;

    // set parameters for sine oscillator object
    osc_sine.Init(sample_rate);
    osc_sine.SetWaveform(Oscillator::WAVE_SIN);
    osc_sine.SetFreq(100);
    osc_sine.SetAmp(0.25);

    // Start callback
    seed.StartAudio(AudioCallback);

    while(1) {}
}
