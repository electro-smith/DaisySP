#include "wavetables.cpp"

// hackish way to make it work for both mcu and computer
#ifndef DSY_SDRAM_BSS
#define DSY_SDRAM_BSS /* emtpy */
#endif

namespace daisysp
{
bool Tables::generated = false;

WaveBuffer DSY_SDRAM_BSS Tables::buffer_pool[40];
uint8_t                  Tables::num_buffers = 0;

WaveTable Tables::Sine;
WaveTable Tables::Square;
WaveTable Tables::Tri;
WaveTable Tables::Saw;

} // namespace daisysp