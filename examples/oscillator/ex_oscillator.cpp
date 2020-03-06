#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;

static daisy_handle seed;
static Oscillator osc;

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
    daisy_seed_init(&seed);
    osc.Init(DSY_AUDIO_SAMPLE_RATE);

    // Set parameters for oscillator
    osc.SetWaveform(osc.WAVE_SIN);
    osc.SetFreq(440);
    osc.SetAmp(0.5);

    // define callback
    dsy_audio_set_callback(DSY_AUDIO_INTERNAL, AudioCallback);

    // start callback
    dsy_audio_start(DSY_AUDIO_INTERNAL);

    while(1) {}
}
