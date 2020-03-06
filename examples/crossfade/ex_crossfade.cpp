#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;

static daisy_handle seed;
static CrossFade cfade;
static Oscillator osc_sine, osc_saw, lfo;

static void AudioCallback(float *in, float *out, size_t size)
{
	float saw, sine, pos, output;
    for (size_t i = 0; i < size; i += 2)
    {
    	sine = osc_sine.Process();
        saw = osc_saw.Process();

        // lfo output = -1 to 1
        pos = lfo.Process();

        // scale signal between 0 and 1
        pos = (pos + 1) / 2;

        cfade.SetPos(pos);
        output = cfade.Process(sine, saw);

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

    // set params for CrossFade object
    cfade.Init();
    cfade.SetCurve(CROSSFADE_LIN);

    // set parameters for sine oscillator object
    osc_sine.Init(DSY_AUDIO_SAMPLE_RATE);
    osc_sine.SetWaveform(Oscillator::WAVE_SIN);
    osc_sine.SetFreq(100);
    osc_sine.SetAmp(0.25);

    // set parameters for sawtooth oscillator object
    osc_saw.Init(DSY_AUDIO_SAMPLE_RATE);
    osc_saw.SetWaveform(Oscillator::WAVE_POLYBLEP_SAW);
    osc_saw.SetFreq(100);
    osc_saw.SetAmp(0.25);

    // set parameters for triangle lfo oscillator object
    lfo.Init(DSY_AUDIO_SAMPLE_RATE);
    lfo.SetWaveform(Oscillator::WAVE_TRI);
    lfo.SetFreq(.25);
    lfo.SetAmp(1);

    // define callback
    dsy_audio_set_callback(DSY_AUDIO_INTERNAL, AudioCallback);

    // start callback
    dsy_audio_start(DSY_AUDIO_INTERNAL);

    while(1) {}
}
