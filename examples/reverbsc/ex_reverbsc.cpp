#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;

static daisy_handle seed;

ReverbSc verb;
 
static void AudioCallback(float *in, float *out, size_t size)
{
    for (size_t i = 0; i < size; i += 2)
    {
        verb.Process(in[i], in[i+1], &out[i], &out[i+1]);
    }
}

int main(void)
{
    // initialize seed hardware and whitenoise daisysp module
    daisy_seed_init(&seed);
    verb.Init(DSY_AUDIO_SAMPLE_RATE);
    verb.SetFeedback(0.85f);
    verb.SetLpFreq(18000.0f);

    // define callback
    dsy_audio_set_callback(DSY_AUDIO_INTERNAL, AudioCallback);

    // start callback
    dsy_audio_start(DSY_AUDIO_INTERNAL);

    while(1) {}
}
