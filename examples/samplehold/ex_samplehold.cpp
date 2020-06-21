#include "daisysp.h"
#include "daisy_seed.h"

// Interleaved audio definitions
#define LEFT (i)
#define RIGHT (i + 1)

using namespace daisysp;
using namespace daisy;

static DaisySeed  seed;
static Oscillator osc;
static Metro      tick;
static WhiteNoise noise;
static AdEnv      env;
static SampleHold sh;

float osc_out = 0;

static void AudioCallback(float *in, float *out, size_t size)
{
    float osc_out, env_out;
    for(size_t i = 0; i < size; i += 2)
    {
        uint8_t trig = tick.Process();
	float newsample = sh.Process(trig, noise.Process() * 500 + 500, sh.MODE_SAMPLE_HOLD);

	// When the metro ticks, trigger the envelope to start.
	if (trig)
	{
	    env.Trigger();
	    tick.SetFreq(newsample / 100 + 1);
	    osc.SetFreq(newsample);
	}
	
        // Use envelope to control the amplitude of the oscillator.
        env_out = env.Process();
        osc.SetAmp(env_out);
        osc_out = osc.Process();

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
    tick.Init(1, sample_rate);

    // set adenv parameters
    env.SetTime(ADENV_SEG_ATTACK, 0.01);
    env.SetTime(ADENV_SEG_DECAY, 0.1);
    env.SetMin(0.0);
    env.SetMax(0.25);
    env.SetCurve(0.5);

    // Set parameters for oscillator
    osc.SetWaveform(osc.WAVE_TRI);
    osc.SetFreq(220);
    osc.SetAmp(0.25);

    noise.Init();

    // start callback
    seed.StartAudio(AudioCallback);


    while(1) {}
}
