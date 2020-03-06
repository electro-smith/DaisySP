#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;

static daisy_handle seed;
static Tone flt;
static Oscillator osc, lfo;

static void AudioCallback(float *in, float *out, size_t size)
{
	float saw, freq, output;
    for (size_t i = 0; i < size; i += 2)
    {
        freq = 2500 + ( lfo.Process()*2500 );
    	saw = osc.Process();

        flt.SetFreq(freq);
        output = flt.Process(saw);

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

    // initialize Tone object
    flt.Init(DSY_AUDIO_SAMPLE_RATE);

    // set parameters for sine oscillator object
    lfo.Init(DSY_AUDIO_SAMPLE_RATE);
    lfo.SetWaveform(Oscillator::WAVE_TRI);
    lfo.SetAmp(1);
    lfo.SetFreq(.4);

    // set parameters for sine oscillator object
    osc.Init(DSY_AUDIO_SAMPLE_RATE);
    osc.SetWaveform(Oscillator::WAVE_POLYBLEP_SAW);
    osc.SetFreq(100);
    osc.SetAmp(0.25);

    // define callback
    dsy_audio_set_callback(DSY_AUDIO_INTERNAL, AudioCallback);

    // start callback
    dsy_audio_start(DSY_AUDIO_INTERNAL);

    while(1) {}
}
