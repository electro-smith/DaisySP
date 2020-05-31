#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;
using namespace daisy;

static DaisySeed  seed;
static Oscillator osc, lfo;
static Bitcrush   bitcrush;
static Metro      metro;

uint8_t     depth;
static void AudioCallback(float *in, float *out, size_t size)
{
    float sig;
    for(size_t i = 0; i < size; i += 2)
    {
        sig = osc.Process();

        if(metro.Process())
        {
            depth++;
            depth %= 6;
        }

        bitcrush.SetBitDepth(depth + 2);
        bitcrush.SetCrushRate((depth + 2) * 2000);
        sig = bitcrush.Process(sig);

        // left out
        out[i] = sig;

        // right out
        out[i + 1] = sig;
    }
}

int main(void)
{
    // initialize seed hardware and oscillator daisysp module
    float sample_rate;
    depth = 1;
    seed.Configure();
    seed.Init();
    sample_rate = seed.AudioSampleRate();
    osc.Init(sample_rate);
    bitcrush.Init(sample_rate);
    metro.Init(1.0f, sample_rate);


    // Set parameters for oscillator
    osc.SetWaveform(osc.WAVE_SIN);
    osc.SetFreq(440);
    osc.SetAmp(0.5);

    //set parameters for bitcrusher
    bitcrush.SetBitDepth(6);
    bitcrush.SetCrushRate(10000);

    // start callback
    seed.StartAudio(AudioCallback);


    while(1) {}
}
