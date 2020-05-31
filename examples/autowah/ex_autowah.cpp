#include "daisysp.h"
#include "daisy_seed.h"

// Shortening long macro for sample rate
#ifndef sample_rate

#endif

// Interleaved audio definitions
#define LEFT (i)
#define RIGHT (i + 1)

using namespace daisysp;
using namespace daisy;

static DaisySeed  seed;
static AdEnv      env;
static Oscillator osc;
static Metro      tick;
static Autowah    autowah;

static void AudioCallback(float *in, float *out, size_t size)
{
    float osc_out, env_out;
    for(size_t i = 0; i < size; i += 2)
    {
        // When the metro ticks, trigger the envelope to start.
        if(tick.Process())
        {
            env.Trigger();
        }

        // Use envelope to control the amplitude of the oscillator.
        // Apply autowah on the signal.
        env_out = env.Process();
        osc.SetAmp(env_out);
        osc_out = osc.Process();
        osc_out = autowah.Process(osc_out);

        out[LEFT]  = osc_out;
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
    autowah.Init(sample_rate);

    // Set up metro to pulse every second
    tick.Init(1.0f, sample_rate);

    // set adenv parameters
    env.SetTime(ADENV_SEG_ATTACK, 0.01);
    env.SetTime(ADENV_SEG_DECAY, 0.5);
    env.SetMin(0.0);
    env.SetMax(0.5);
    env.SetCurve(0); // linear

    // Set parameters for oscillator
    osc.SetWaveform(osc.WAVE_SAW);
    osc.SetFreq(220);
    osc.SetAmp(0.25);

    //set autowah parameters
    autowah.SetLevel(.1);
    autowah.SetDryWet(100);
    autowah.SetWah(1);


    // start callback
    seed.StartAudio(AudioCallback);


    while(1) {}
}
