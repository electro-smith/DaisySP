#include "daisysp.h"
#include "daisy_seed.h"

// Shortening long macro for sample rate
#ifndef sample_rate

#endif

// Interleaved audio definitions
#define LEFT (i)
#define RIGHT (i + 1)

using namespace daisysp;
using namespace daisy;

static DaisySeed seed;
static Drip      drip;
static Metro     tick;
bool             gate;

static void AudioCallback(float *in, float *out, size_t size)
{
    float output;
    bool  trig;
    for(size_t i = 0; i < size; i += 2)
    {
        // When the metro ticks, trigger the envelope to start.
        if(tick.Process())
        {
            trig = true;
        }
        else
        {
            trig = false;
        }

        output = drip.Process(trig);

        out[LEFT]  = output;
        out[RIGHT] = output;
    }
}

int main(void)
{
    // initialize seed hardware and daisysp modules
    float sample_rate;
    seed.Configure();
    seed.Init();
    sample_rate = seed.AudioSampleRate();

    // Set up metro to pulse every second
    tick.Init(1.0f, sample_rate);

    drip.Init(sample_rate, .1);

    // start callback
    seed.StartAudio(AudioCallback);

    while(1) {}
}
