#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;
using namespace daisy;

static DaisySeed         seed;
static FormantOscillator form[2];
static Metro             tick;
static Oscillator        lfo;

float freqs[3][2] = {{390, 2300}, {610, 1900}, {820, 1530}};
int   idx         = 0;

static void AudioCallback(float *in, float *out, size_t size)
{
    for(size_t i = 0; i < size; i += 2)
    {
        if(tick.Process())
        {
            idx = (idx + 1) % 3;
        }

        form[0].SetPhaseShift(lfo.Process());
        form[1].SetPhaseShift(lfo.Process());

        form[0].SetFormantFreq(freqs[idx][0]);
        form[1].SetFormantFreq(freqs[idx][1]);
        out[i] = out[i + 1]
            = (form[0].Process() * .6f + form[1].Process() * .4f);
    }
}

int main(void)
{
    // initialize seed hardware and oscillator daisysp module
    float sample_rate;
    seed.Configure();
    seed.Init();
    sample_rate = seed.AudioSampleRate();

    form[0].Init(sample_rate);
    form[0].SetCarrierFreq(200.f);

    form[1].Init(sample_rate);
    form[1].SetCarrierFreq(200.f);

    lfo.Init(sample_rate);
    lfo.SetAmp(1.f);
    lfo.SetFreq(.2f);

    tick.Init(1.f, sample_rate);

    // start callback
    seed.StartAudio(AudioCallback);

    while(1) {}
}
