#include "daisysp.h"
#include "daisy_seed.h"

// Shortening long macro for sample rate
#ifndef SAMPLE_RATE
#define SAMPLE_RATE DSY_AUDIO_SAMPLE_RATE
#endif

// Interleaved audio definitions
#define LEFT (i)
#define RIGHT (i+1)

using namespace daisysp;

static daisy_handle seed;
static AdEnv env;
static Oscillator osc;
static Metro tick;

static void AudioCallback(float *in, float *out, size_t size)
{
	float osc_out, env_out;
    for (size_t i = 0; i < size; i += 2)
    {
        // When the metro ticks, trigger the envelope to start.
        if (tick.Process())
        {
            env.Trigger();
        }

        // Use envelope to control the amplitude of the oscillator.
        env_out = env.Process();
        osc.SetAmp(env_out);
    	osc_out = osc.Process();

        out[LEFT] = osc_out;
        out[RIGHT] = osc_out;
    }
}

int main(void)
{
    // initialize seed hardware and daisysp modules
    daisy_seed_init(&seed);
    env.Init(SAMPLE_RATE);
    osc.Init(SAMPLE_RATE);

    // Set up metro to pulse every second
    tick.Init(1.0f, SAMPLE_RATE);    

    // set adenv parameters
    env.SetTime(ADENV_SEG_ATTACK, 0.15);
    env.SetTime(ADENV_SEG_DECAY, 0.35);
    env.SetMin(0.0);
    env.SetMax(0.25);
    env.SetCurve(0); // linear

    // Set parameters for oscillator
    osc.SetWaveform(osc.WAVE_TRI);
    osc.SetFreq(220);
    osc.SetAmp(0.25);

    // define callback
    dsy_audio_set_callback(DSY_AUDIO_INTERNAL, AudioCallback);

    // start callback
    dsy_audio_start(DSY_AUDIO_INTERNAL);

    while(1) {}
}
