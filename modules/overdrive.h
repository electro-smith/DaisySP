#pragma once
#ifndef DSY_OVERDRIVE_H
#define DSY_OVERDRIVE_H

#include <stdint.h>
#ifdef __cplusplus

/** @file overdrive.h */

namespace daisysp
{
/**  
	   Distortion / Overdrive \n
       Ported by Ben Sergentanis, Jan 2021 
       @author Emilie Gillet
       @date 2014
*/
class Overdrive
{
  public:
    Overdrive() {}
    ~Overdrive() {}

    /** Initializes the module with 0 gain */
    void Init();

    /** Get the next sample
		\param in Input to be overdriven
	*/
    float Process(float in);

    /** Set the amount of drive
		\param drive Works from 0-1
	*/
    void SetDrive(float drive);

  private:
    inline float SoftClip(float x);
    inline float SoftLimit(float x);

    float drive_;
    float pre_gain_;
    float post_gain_;
};
} // namespace daisysp
#endif
#endif