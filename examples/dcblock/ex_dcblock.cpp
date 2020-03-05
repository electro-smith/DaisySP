#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;

static daisy_handle seed;
static DcBlock block;
static Oscillator osc_sine;

static void AudioCallback(float *in, float *out, size_t size)
{
	float output;
    for (size_t i = 0; i < size; i += 2)
    {
    	output = osc_sine.Process();

        // add dc to signal
        output += 1;

        // remove dc from signal
        output = block.Process(output);

    	// left out
        out[i] = output;

        // right out
        out[i+1] = output;
    }
}

int main(void)
{
	// initialize seed hardware and daisysp modules
    daisy_seed_init(&seed);

    block.Init(DSY_AUDIO_SAMPLE_RATE);

    // set parameters for sine oscillator object
    osc_sine.Init(DSY_AUDIO_SAMPLE_RATE);
    osc_sine.SetWaveform(Oscillator::WAVE_SIN);
    osc_sine.SetFreq(100);
    osc_sine.SetAmp(0.25);

    // define callback
    dsy_audio_set_callback(DSY_AUDIO_INTERNAL, AudioCallback);

    // start callback
    dsy_audio_start(DSY_AUDIO_INTERNAL);

    while(1) {}
}
