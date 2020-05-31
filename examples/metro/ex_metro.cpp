#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;
using namespace daisy;

static DaisySeed  seed;
static Metro      clock;
static Oscillator osc_sine;

static void AudioCallback(float *in, float *out, size_t size)
{
    float   sine, freq;
    uint8_t tic;
    for(size_t i = 0; i < size; i += 2)
    {
        tic = clock.Process();
        if(tic)
        {
            freq = rand() % 500;
            osc_sine.SetFreq(freq);
        }

        sine = osc_sine.Process();

        // left out
        out[i] = sine;

        // right out
        out[i + 1] = sine;
    }
}

int main(void)
{
    // initialize seed hardware and daisysp modules
    float sample_rate;
    seed.Configure();
    seed.Init();
    sample_rate = seed.AudioSampleRate();

    // initialize Metro object at 2 hz
    clock.Init(2, sample_rate);

    // set parameters for sine oscillator object
    osc_sine.Init(sample_rate);
    osc_sine.SetWaveform(Oscillator::WAVE_SIN);
    osc_sine.SetFreq(100);
    osc_sine.SetAmp(0.25);


    // start callback
    seed.StartAudio(AudioCallback);


    while(1)
    {
    }
}
