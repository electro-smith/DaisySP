#include <string.h>
#include "daisy_seed.h"

// Global Init Structure for Daisy 
daisy_handle seed;

static void Callback(float *in, float *out, size_t size)
{
    memcpy(out, in, size*sizeof(float));
}
int main(void)
{
    daisy_seed_init(&seed);    
    dsy_audio_set_callback(DSY_AUDIO_INTERNAL, Callback);
    dsy_audio_start(DSY_AUDIO_INTERNAL);
    while(1) 
    {
    }
}
