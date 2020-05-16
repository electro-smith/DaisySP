// # Decimator
// Performs downsampling and bitcrush effects

#pragma once
#ifndef DECIMATOR_H
#define DECIMATOR_H
#include <stdint.h>
#ifdef __cplusplus

namespace daisysp
{
	class Decimator
	{
	public:
		Decimator() {}
		~Decimator() {}

// ### Init
// Initializes downsample module
// ~~~~
		void Init();
// ~~~~

// ### Process
// Applies downsample and bitcrush effects to input signal.
// Returns one sample. This should be called once per sample period. 
// ~~~~
		float Process(float input);
// ~~~~

// ## Mutators

// ### SetDownsampleFactor
// Sets amount of downsample 
// Input range: 

// ~~~~
		inline void SetDownsampleFactor(float downsample_factor) 
// ~~~~
		{
			downsample_factor_ = downsample_factor;
		}

// ### SetBitcrushFactor
// Sets amount of bitcrushing 
// Input range: 

// ~~~~
		inline void SetBitcrushFactor(float bitcrush_factor)
// ~~~~
		{
			//			bitcrush_factor_ = bitcrush_factor;
			bits_to_crush_ = bitcrush_factor * kMaxBitsToCrush;
		}

// ### SetBitsToCrush
// Sets the exact number of bits to crush
// 
// 0-16 bits
// ~~~~
		inline void SetBitsToCrush(const uint8_t &bits) 
// ~~~~
		{
			bits_to_crush_ = bits <= kMaxBitsToCrush ? bits : kMaxBitsToCrush;
		}

// ## Accessors

// ### GetDownsampleFactor
// Returns current setting of downsample

// ~~~~
		inline float GetDownsampleFactor() { return downsample_factor_; }
// ~~~~

// ### GetBitcrushFactor
// Returns current setting of bitcrush

// ~~~~
		inline float GetBitcrushFactor() { return bitcrush_factor_; }
// ~~~~

	  private:
		const uint8_t kMaxBitsToCrush = 16;
		float downsample_factor_, bitcrush_factor_;
		uint32_t bits_to_crush_;
		float downsampled_, bitcrushed_;
		uint32_t inc_, threshold_;
	};
} // namespace daisysp
#endif
#endif
