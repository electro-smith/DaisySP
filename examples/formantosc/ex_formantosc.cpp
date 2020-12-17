#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;
using namespace daisy;

static DaisySeed  seed;
static FormantOscillator  form;
static Oscillator lfo[2];

static void AudioCallback(float *in, float *out, size_t size)
{
  for(size_t i = 0; i < size; i += 2)
    {
		float lsig1 = fabsf(lfo[0].Process());
		float lsig2 = fabsf(lfo[1].Process());

		out[i] = out[i + 1] = form.Process(lsig1 * .01f, lsig2 * .05f, .5f);
    }
}

int main(void)
{
    // initialize seed hardware and oscillator daisysp module
    float sample_rate;
    seed.Configure();
    seed.Init();
    sample_rate = seed.AudioSampleRate();
    form.Init();

	lfo[0].Init(sample_rate);
	lfo[0].SetAmp(1.f);
	lfo[0].SetFreq(.05f);

	lfo[1].Init(sample_rate);
	lfo[1].SetAmp(1.f);
	lfo[1].SetFreq(.25f);


    // start callback
    seed.StartAudio(AudioCallback);


    while(1) {}
}
