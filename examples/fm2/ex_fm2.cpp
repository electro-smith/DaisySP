#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;
using namespace daisy;

static DaisySeed  seed;
Fm2 Osc;
Oscillator lfo1, lfo2;

static void AudioCallback(float *in, float *out, size_t size)
{
    float output;
    for(size_t i = 0; i < size; i += 2)
    {
		output = Osc.Process();
        out[i] = out[i + 1] = output;
    }
}

int main(void)
{
    // initialize seed hardware and daisysp modules
    float sample_rate;
    seed.Configure();
    seed.Init();
    sample_rate = seed.AudioSampleRate();

	Osc.Init(sample_rate);
	
    // start callback
    seed.StartAudio(AudioCallback);
    while(1) {}
}
