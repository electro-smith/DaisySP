// Example that takes the mono input from channel 1 (left input), 
// and pitchshifts it up 1 octave.
// The left output will be pitchshifteed, while the right output
// stays will be the unshifted left input.

#include "daisysp.h"
#include "daisy_seed.h"

// Defines for Interleaved Audio
#define LEFT (i)
#define RIGHT (i+1)

using namespace daisysp;
using namespace daisy;

DaisySeed seed;
PitchShifter ps;

static void AudioCallback(float *in, float *out, size_t size)
{
	float shifted, unshifted;
    for (size_t i = 0; i < size; i += 2)
    {
        unshifted = in[LEFT];
        shifted = ps.Process(unshifted);
        out[LEFT] = shifted;
        out[RIGHT] = unshifted;
    }
}

int main(void)
{
	// initialize seed hardware and daisysp modules
    float sample_rate;
	seed.Configure();
	seed.Init();
	sample_rate = seed.AudioSampleRate();

    ps.Init(sample_rate);
    // set transposition 1 octave up (12 semitones)
    ps.SetTransposition(12.0f);

    // start callback
	seed.StartAudio(AudioCallback);


    while(1) {}
}
