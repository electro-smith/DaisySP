#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;
using namespace daisy;

static DaisySeed  seed;
static FormantOscillator  form;

static void AudioCallback(float *in, float *out, size_t size)
{
  for(size_t i = 0; i < size; i += 2)
    {
        out[i] = out[i + 1] = 0.f;
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

    // start callback
    seed.StartAudio(AudioCallback);


    while(1) {}
}
