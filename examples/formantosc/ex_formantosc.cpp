#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;
using namespace daisy;

static DaisySeed  seed;
static FormantOscillator  form;
static Oscillator lfo[3];

static void AudioCallback(float *in, float *out, size_t size)
{
  for(size_t i = 0; i < size; i += 2)
    {
		float lsig1 = fabsf(lfo[0].Process());
		float lsig2 = 1 +  4 * fabsf(lfo[1].Process());
		float lsig3 = fabsf(lfo[2].Process());

		//out[i] = out[i + 1] = form.Process(.005f, lsig2 *.002f, .75f);
		
		out[i] = out[i + 1] = form.Process(440.f, 440.f, .00f);

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

	lfo[0].Init(sample_rate);
	lfo[0].SetAmp(1.f);
	lfo[0].SetFreq(.05f);

	lfo[1].Init(sample_rate);
	lfo[1].SetAmp(1.f);
	lfo[1].SetFreq(.25f);

	lfo[2].Init(sample_rate);
	lfo[2].SetAmp(1.f);
	lfo[2].SetFreq(.5f);

    // start callback
    seed.StartAudio(AudioCallback);


    while(1) {}
}
