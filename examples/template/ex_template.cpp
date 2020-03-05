#include "daisy_seed.h"

static daisy_handle seed;

int main(void)
{
    daisy_seed_init(&seed);
    while(1) {}
}
