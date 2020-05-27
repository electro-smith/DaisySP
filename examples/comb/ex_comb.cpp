#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;
using namespace daisy;

static DaisySeed seed;
static Comb flt;
static WhiteNoise noise;

static void AudioCallback(float *in, float *out, size_t size)
{
	float output;
    for (size_t i = 0; i < size; i += 2)
    {
        output = noise.Process();

	//flt.SetLoopTime(time);
        output = 0.5 * flt.Process(output);

    	// left out
        out[i] = output;

        // right out
        out[i+1] = output;
    }
}

int main(void)
{
	// initialize seed hardware and daisysp modules
    float sample_rate;
	seed.Configure();
	seed.Init();
	sample_rate = seed.AudioSampleRate();

	// initialize Comb object
	flt.Init(sample_rate);

	//initialize noise
	noise.Init();
	
	flt.SetLoopTime(.01);
    

	// start callback
	seed.StartAudio(AudioCallback);


    while(1) {}
}
