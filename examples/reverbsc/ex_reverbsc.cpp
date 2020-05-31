#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;
using namespace daisy;

static DaisySeed seed;

ReverbSc verb;

static void AudioCallback(float *in, float *out, size_t size)
{
    for(size_t i = 0; i < size; i += 2)
    {
        verb.Process(in[i], in[i + 1], &out[i], &out[i + 1]);
    }
}

int main(void)
{
    // initialize seed hardware and whitenoise daisysp module
    float sample_rate;
    seed.Configure();
    seed.Init();
    sample_rate = seed.AudioSampleRate();
    verb.Init(sample_rate);
    verb.SetFeedback(0.85f);
    verb.SetLpFreq(18000.0f);


    // start callback
    seed.StartAudio(AudioCallback);


    while(1) {}
}
