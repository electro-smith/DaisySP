#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;
using namespace daisy;

static DaisySeed  seed;
static WhiteNoise nse;

static void AudioCallback(float *in, float *out, size_t size)
{
    float sig;

    for(size_t i = 0; i < size; i += 2)
    {
        sig = nse.Process();

        // left out
        out[i] = sig;

        // right out
        out[i + 1] = sig;
    }
}

int main(void)
{
    // initialize seed hardware and WhiteNoise daisysp module
    seed.Configure();
    seed.Init();
    nse.Init();

    // start callback
    seed.StartAudio(AudioCallback);

    while(1)
    {
    }
}
