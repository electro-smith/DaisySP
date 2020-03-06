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
static Oscillator osc;
static Decimator decim;
static Phasor phs;

static void AudioCallback(float *in, float *out, size_t size)
{
	float osc_out, decimated_out;
    float downsample_amt;
    for (size_t i = 0; i < size; i += 2)
    {
        // Generate a pure sine wave
    	osc_out = osc.Process();
        // Modulate downsample amount via Phasor
        downsample_amt = phs.Process();
        decim.SetDownsampleFactor(downsample_amt);
        // downsample and bitcrush
        decimated_out = decim.Process(osc_out);
        // outputs
        out[LEFT] = decimated_out;
        out[RIGHT] = decimated_out;
    }
}

int main(void)
{
    // initialize seed hardware and daisysp modules
    daisy_seed_init(&seed);
    osc.Init(SAMPLE_RATE);
    phs.Init(SAMPLE_RATE, 0.5f);
    decim.Init();

    // Set parameters for oscillator
    osc.SetWaveform(osc.WAVE_SIN);
    osc.SetFreq(220);
    osc.SetAmp(0.25);
    // Set downsampling, and bit crushing values.
    decim.SetDownsampleFactor(0.4f);
    decim.SetBitsToCrush(8);

    // define callback
    dsy_audio_set_callback(DSY_AUDIO_INTERNAL, AudioCallback);

    // start callback
    dsy_audio_start(DSY_AUDIO_INTERNAL);

    while(1) {}
}
