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

// Helper Modules
static AdEnv env;
static Oscillator osc;
static Metro tick;

static NlFilt filt;

static void AudioCallback(float *in, float *out, size_t size)
{
    // The NlFilt object currently only works on blocks of audio at a time.
    // This can be accomodated easily with an extra loop at the end.
    // We use size/2 since we only need to process mono
    float dry[size/2];
    float wet[size/2];
	float env_out;
    // loop through mono process
    for (size_t i = 0; i < size/2; i++)
    {
        // When the Metro ticks: 
        // trigger the envelope to start, and change freq of oscillator.
        if (tick.Process())
        {
            float freq = rand() % 150;
            osc.SetFreq(freq + 25.0f);
            env.Trigger();
        }
        // Use envelope to control the amplitude of the oscillator.
        env_out = env.Process();
        osc.SetAmp(env_out);
    	dry[i] = osc.Process();
    }
    // nonlinear filter
    filt.ProcessBlock(dry, wet, size/2);
    // Now write wet signal to both outputs.
    for (size_t i = 0; i < size; i+=2)
    {
        out[LEFT] = wet[i/2];
        out[RIGHT] = wet[i/2];
    }
}

int main(void)
{
    // initialize seed hardware and daisysp modules
    daisy_seed_init(&seed);
    env.Init(SAMPLE_RATE);
    osc.Init(SAMPLE_RATE);

    // Set up Metro to pulse every 3 seconds
    tick.Init(0.333f, SAMPLE_RATE);    

    // Set adenv parameters
    env.SetTime(ADENV_SEG_ATTACK, 1.50);
    env.SetTime(ADENV_SEG_DECAY, 1.50);
    env.SetMin(0.0);
    env.SetMax(0.25);
    env.SetCurve(0); // linear

    // Set parameters for oscillator
    osc.SetWaveform(osc.WAVE_POLYBLEP_SAW);

    // Set coefficients for non-linear filter.
    filt.SetCoefficients(0.7f, -0.2f, 0.95f, 0.24f, 1000.0f);

    // define callback
    dsy_audio_set_callback(DSY_AUDIO_INTERNAL, AudioCallback);

    // start callback
    dsy_audio_start(DSY_AUDIO_INTERNAL);

    while(1) {}
}
