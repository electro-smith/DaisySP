#include "daisy_seed.h"

using namespace daisy;

static DaisySeed seed;

int main(void)
{
	seed.Configure();
	seed.Init();
    while(1) {}
}
