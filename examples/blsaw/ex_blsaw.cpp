#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;
using namespace daisy;

static DaisySeed seed;
static BlSaw osc;

static void AudioCallback(float *in, float *out, size_t size)
{
	float sig;
    for (size_t i = 0; i < size; i += 2)
    {
    	sig = osc.Process();

    	// left out
        out[i] = sig;

        // right out
        out[i+1] = sig;
    }
}

int main(void)
{
	// initialize seed hardware and oscillator daisysp module
    float sample_rate;
	seed.Configure();
	seed.Init();
	sample_rate = seed.AudioSampleRate();
	osc.Init(sample_rate);

    // Set parameters for oscillator;
    osc.SetFreq(440);
    osc.SetAmp(0.5);

    
    

    // start callback
	seed.StartAudio(AudioCallback);


    while(1) {}
}
