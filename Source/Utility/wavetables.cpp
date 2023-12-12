#include "wavetables.h"

// hackish way to make it work for both mcu and computer
#ifndef DSY_SDRAM_BSS
#define DSY_SDRAM_BSS /* emtpy */
#endif

namespace daisysp
{

template<typename T, FFTFunction<T> fft> 
bool Tables<T, fft>::generated = false;

template<typename T, FFTFunction<T> fft>
WaveBuffer DSY_SDRAM_BSS Tables<T, fft>::buffer_pool[40];
template<typename T, FFTFunction<T> fft>
uint8_t                  Tables<T, fft>::num_buffers = 0;

template<typename T, FFTFunction<T> fft>
WaveTable Tables<T, fft>::Sine;
template<typename T, FFTFunction<T> fft>
WaveTable Tables<T, fft>::Square;
template<typename T, FFTFunction<T> fft>
WaveTable Tables<T, fft>::Tri;
template<typename T, FFTFunction<T> fft>
WaveTable Tables<T, fft>::Saw;

} // namespace daisysp
