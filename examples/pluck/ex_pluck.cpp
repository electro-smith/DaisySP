#include "daisysp.h"
#include "daisy_seed.h"
#include <algorithm>

// Shortening long macro for sample rate
#ifndef SAMPLE_RATE
#define SAMPLE_RATE DSY_AUDIO_SAMPLE_RATE
#endif

// Interleaved audio definitions
#define LEFT (i)
#define RIGHT (i+1)

using namespace daisysp;

static daisy_handle seed;

// Helper Modules
static Metro tick;
static Pluck plk;

// MIDI note numbers for a major triad
const float kArpeggio[3] = { 48.0f, 52.0f, 55.0f };
uint8_t arp_idx;

static void AudioCallback(float *in, float *out, size_t size)
{
	float sig_out, freq, trig;
    for (size_t i = 0; i < size; i += 2)
    {
        // When the Metro ticks: 
        // advance the kArpeggio, and trigger the Pluck.
        trig = 0.0f;
        if (tick.Process())
        {
            freq = mtof(kArpeggio[arp_idx]); // convert midi nn to frequency.
            arp_idx = (arp_idx + 1) % 3; // advance the kArpeggio, wrapping at the end.
            plk.SetFreq(freq);
            trig = 1.0f;
        }
        sig_out = plk.Process(trig);
        // Output
        out[LEFT] = sig_out;
        out[RIGHT] = sig_out;
    }
}

int main(void)
{
    float init_buff[256]; // buffer for Pluck impulse

    // initialize seed hardware and daisysp modules
    daisy_seed_init(&seed);

    // Set up Metro to pulse every second
    tick.Init(1.0f, SAMPLE_RATE);    
    // Set up Pluck algo
    plk.Init(SAMPLE_RATE, init_buff, 256, PLUCK_MODE_RECURSIVE);
    plk.SetDecay(0.95f);
    plk.SetDamp(0.9f);
    plk.SetAmp(0.3f);

    arp_idx = 0;

    // define callback
    dsy_audio_set_callback(DSY_AUDIO_INTERNAL, AudioCallback);

    // start callback
    dsy_audio_start(DSY_AUDIO_INTERNAL);

    while(1) {}
}
