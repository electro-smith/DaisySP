#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;

static daisy_handle seed;
static Port slew;
static Metro clock;
static Oscillator osc_sine;

float freq;

static void AudioCallback(float *in, float *out, size_t size)
{
    float sine, slewed_freq;
    uint8_t tic;
    for (size_t i = 0; i < size; i += 2)
    {   
        tic = clock.Process();
        if (tic)
        {
            freq = rand() % 500;
        }

        slewed_freq = slew.Process(freq);
        osc_sine.SetFreq(slewed_freq);

        sine = osc_sine.Process();

        // left out
        out[i] = sine;

        // right out
        out[i+1] = sine;
    }
}

int main(void)
{
    // initialize seed hardware and daisysp modules
    daisy_seed_init(&seed);

    // set params for Port object
    slew.Init(DSY_AUDIO_SAMPLE_RATE, .09);

    clock.Init(1, DSY_AUDIO_SAMPLE_RATE);

    // set parameters for sine oscillator object
    osc_sine.Init(DSY_AUDIO_SAMPLE_RATE);
    osc_sine.SetWaveform(Oscillator::WAVE_SIN);
    osc_sine.SetFreq(100);
    osc_sine.SetAmp(0.25);

    // define callback
    dsy_audio_set_callback(DSY_AUDIO_INTERNAL, AudioCallback);

    // start callback
    dsy_audio_start(DSY_AUDIO_INTERNAL);

    while(1) {}
}
