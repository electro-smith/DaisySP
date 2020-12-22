#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;
using namespace daisy;

static DaisySeed  seed;
static FormantOscillator  form;
static Oscillator lfo;

static void AudioCallback(float *in, float *out, size_t size)
{
  for(size_t i = 0; i < size; i += 2)
    {
		float sig = 800.f *  fabsf(lfo.Process()) + 200;
		form.SetFormantFreq(sig);
				
		out[i] = out[i + 1] = form.Process();
    }
}

int main(void)
{
    // initialize seed hardware and oscillator daisysp module
    float sample_rate;
    seed.Configure();
    seed.Init();
    sample_rate = seed.AudioSampleRate();
    form.Init(sample_rate);
	form.SetCarrierFreq(197.f);
	form.SetPhaseShift(.75f);

	lfo.Init(sample_rate);
	lfo.SetAmp(1.f);
	lfo.SetFreq(.25f);

    // start callback
    seed.StartAudio(AudioCallback);

    while(1) {}
}
