#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;
using namespace daisy;

static DaisySeed seed;
static Oscillator osc1, osc2;
static Bal bal;

static void AudioCallback(float *in, float *out, size_t size)
{
  float sig1, sig2;
    for (size_t i = 0; i < size; i += 2)
    {
    	sig1 = osc1.Process();
	sig2 = osc2.Process();
	sig1 = bal.Process(sig1, sig2);
	
    	// left out
        out[i] = sig1;

        // right out
        out[i+1] = sig1;
    }
}

int main(void)
{
	// initialize seed hardware and oscillator daisysp module
    float sample_rate;
	seed.Configure();
	seed.Init();
	sample_rate = seed.AudioSampleRate();
	osc1.Init(sample_rate);
	osc2.Init(sample_rate);
	bal.Init(sample_rate);

	
    // Set parameters for oscillator one
    osc1.SetWaveform(osc1.WAVE_SIN);
    osc1.SetFreq(440);
    osc1.SetAmp(0.1);

    // Set parameters for oscillator two
    osc2.SetWaveform(osc2.WAVE_SIN);
    osc2.SetFreq(220);
    osc2.SetAmp(0.5);
 
    // start callback
	seed.StartAudio(AudioCallback);


    while(1) {}
}
