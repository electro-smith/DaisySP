#include <string.h>
#include "daisy_seed.h"

using namespace daisy;

DaisySeed seed;

static void Callback(float *in, float *out, size_t size)
{
    memcpy(out, in, size * sizeof(float));
}

int main(void)
{
    seed.Configure();
    seed.Init();
    seed.StartAudio(Callback);
    while(1)
    {
    }
}
