#pragma once
#ifndef DSY_CHORUS_H
#define DSY_CHORUS_H

#include <stdint.h>
#ifdef __cplusplus

/** @file chorus.h */

namespace daisysp
{
/**  
    @brief Chorus Effect.
	@author Ben Sergentanis
	@date Jan 2021
	Based on https://www.izotope.com/en/learn/understanding-chorus-flangers-and-phasers-in-audio-production.html \n
*/
class Chorus
{
  public:
    Chorus() {}
    ~Chorus() {}
	
	void Init(float sample_rate);
	
	float Process(float in);
	
  private:
	float sample_rate_;
};
} //namespace daisysp
#endif
#endif