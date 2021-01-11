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
class Overdrive {
 public:
  Overdrive() { }
  ~Overdrive() { }
  
  void Init();
  
  float Process(float in);
  
  void SetDrive(float drive);
  
 private:
 inline float SoftClip(float x);
 inline float SoftLimit(float x);
 
  float drive_;
  float pre_gain_;
  float post_gain_;
  
};
}  // namespace daisysp
#endif
#endif