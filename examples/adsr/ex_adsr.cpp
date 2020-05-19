// adsr envelope example

#include "daisysp.h"
#include "daisy_seed.h"

// Shortening long macro for sample rate
#ifndef sample_rate

#endif

// Interleaved audio definitions
#define LEFT (i)
#define RIGHT (i+1)

using namespace daisysp;
using namespace daisy;

static DaisySeed seed;
static Adsr env;
static Oscillator osc;
static Metro tick;
bool gate;

static void AudioCallback(float *in, float *out, size_t size)
{
    float osc_out, env_out;
    for (size_t i = 0; i < size; i += 2)
    {
        // When the metro ticks, trigger the envelope to start.
        if (tick.Process())
        {
            gate = !gate;
	}

        // Use envelope to control the amplitude of the oscillator.
        env_out = env.Process(gate);
        osc.SetAmp(env_out);
    	osc_out = osc.Process();

        out[LEFT] = osc_out;
        out[RIGHT] = osc_out;
    }
}

int main(void)
{
    // initialize seed hardware and daisysp modules
    float sample_rate;
	seed.Configure();
	seed.Init();
	sample_rate = seed.AudioSampleRate();
    env.Init(sample_rate);
    osc.Init(sample_rate);

    // Set up metro to pulse every second
    tick.Init(1.0f, sample_rate);    

    //Set envelope parameters
    env.SetTime( ADSR_SEG_ATTACK, .1);
    env.SetTime( ADSR_SEG_DECAY, .1);
    env.SetTime( ADSR_SEG_RELEASE, .01);

    env.SetSustainLevel(.25);
    
    // Set parameters for oscillator
    osc.SetWaveform(osc.WAVE_TRI);
    osc.SetFreq(220);
    osc.SetAmp(0.25);

    // start callback
	seed.StartAudio(AudioCallback);

    while(1) {}
}
