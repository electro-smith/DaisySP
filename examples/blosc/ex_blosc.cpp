#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;
using namespace daisy;

static DaisySeed seed;
static BlOsc osc;
static Metro tick;

uint8_t waveform;
static void AudioCallback(float *in, float *out, size_t size)
{
	float sig;
    for (size_t i = 0; i < size; i += 2)
    {
      //switch waveforms
        if (tick.Process())
	{
	  waveform++;
	  osc.SetWaveform(waveform%3);
	}
	
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

    // Set up metro to pulse every second
    tick.Init(1.0f, sample_rate);    
	
    // Set parameters for oscillator;
    osc.SetFreq(440);
    osc.SetAmp(0.5);
    osc.SetPw(.5);
    
    // start callback
	seed.StartAudio(AudioCallback);


    while(1) {}
}
