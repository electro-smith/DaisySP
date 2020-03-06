#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisy;
using namespace daisysp;

static daisy_handle seed;

Oscillator osc;
Svf filt;

static void AudioCallback(float *in, float *out, size_t size)
{
    float sig;

    for (size_t i = 0; i < size; i += 2)
    {
        sig = osc.Process();

        filt.Process(sig);

        // left out
        out[i] = filt.Low();

        // right out
        out[i + 1] = filt.High();
    }
}

int main(void)
{
    // initialize seed hardware and Svf daisysp module
    daisy_seed_init(&seed);
    // Initialize Oscillator, and set parameters.
    osc.Init(DSY_AUDIO_SAMPLE_RATE);
    osc.SetWaveform(osc.WAVE_POLYBLEP_SAW);
    osc.SetFreq(250.0);
    osc.SetAmp(0.5);
    // Initialize Filter, and set parameters.
    filt.Init(DSY_AUDIO_SAMPLE_RATE);
    filt.SetFreq(500.0);
    filt.SetRes(0.85);
    filt.SetDrive(0.8);

    // define callback
    dsy_audio_set_callback(DSY_AUDIO_INTERNAL, AudioCallback);

    dsy_adc_start();

    // start callback
    dsy_audio_start(DSY_AUDIO_INTERNAL);

    while(1) {}
}
