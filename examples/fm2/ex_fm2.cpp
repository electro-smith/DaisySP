#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;
using namespace daisy;

static DaisySeed  seed;
Fm2 Osc;
Oscillator lfo1, lfo2, lfo3;

static void AudioCallback(float *in, float *out, size_t size)
{
    float output;
    for(size_t i = 0; i < size; i += 2)
    {
		float lfo1out = lfo1.Process();
		float lfo2out = lfo2.Process();
		float lfo3out = lfo3.Process();

		Osc.SetIndex(lfo1out * 800.f);
		Osc.SetModulatorFrequency(lfo2out * 400.f + 100);
		Osc.SetCarrierFrequency(lfo3out * 200.f + 400.f);
		
		output = Osc.Process();
        out[i] = out[i + 1] = output;
    }
}

int main(void)
{
    // initialize seed hardware and daisysp modules
    float sample_rate;
    seed.Configure();
    seed.Init();
    sample_rate = seed.AudioSampleRate();

	Osc.Init(sample_rate);	
	lfo1.Init(sample_rate);
	lfo2.Init(sample_rate);
	lfo3.Init(sample_rate);

	
	lfo1.SetFreq(.25f);
	lfo2.SetFreq(.1f);
	lfo3.SetFreq(.5f);
	lfo3.SetWaveform(Oscillator::WAVE_SQUARE);
	
	
    // start callback
    seed.StartAudio(AudioCallback);
    while(1) {}
}
