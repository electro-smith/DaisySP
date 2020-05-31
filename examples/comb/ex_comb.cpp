#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;
using namespace daisy;

static DaisySeed  seed;
Comb              flt;
static WhiteNoise noise;

float buf[9600];

static void AudioCallback(float *in, float *out, size_t size)
{
    float output;
    for(size_t i = 0; i < size; i += 2)
    {
        output = noise.Process();

        output = 0.5 * flt.Process(output);

        // left out
        out[i] = output;

        // right out
        out[i + 1] = output;
    }
}

int main(void)
{
    // initialize seed hardware and daisysp modules
    float sample_rate;
    seed.Configure();
    seed.Init();
    sample_rate = seed.AudioSampleRate();

    for(int i = 0; i < 9600; i++)
    {
        buf[i] = 0.0f;
    }

    // initialize Comb object
    flt.Init(sample_rate, buf, 9600);
    flt.SetFreq(.01);

    //initialize noise
    noise.Init();

    // start callback
    seed.StartAudio(AudioCallback);


    while(1) {}
}
