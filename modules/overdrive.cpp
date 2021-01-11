#include "dsp.h"
#include "overdrive.h"

using namespace daisysp;

  void Overdrive::Init() {
    pre_gain_ = 0.0f;
    post_gain_ = 0.0f;
  }
  
  float Overdrive::Process(float in) {
    const float drive_2 = drive_ * drive_;
    const float pre_gain_a = drive_ * 0.5f;
    const float pre_gain_b = drive_2 * drive_2 * drive_ * 24.0f;
    const float pre_gain_ = pre_gain_a + (pre_gain_b - pre_gain_a) * drive_2;
    const float drive_squashed = drive_ * (2.0f - drive_);
    const float post_gain_ = 1.0f / SoftClip(
          0.33f + drive_squashed * (pre_gain_ - 0.33f));
        
      float pre = pre_gain_ * in;
      return SoftClip(pre) * post_gain_;
    }
	
	
	void Overdrive::SetDrive(float drive){
		drive_ = drive;
	}
	
  inline float Overdrive::SoftLimit(float x) {
	return x * (27.0f + x * x) / (27.0f + 9.0f * x * x);
  }
	
  inline float Overdrive::SoftClip(float x) {
  if (x < -3.0f) {
    return -1.0f;
  } else if (x > 3.0f) {
    return 1.0f;
  } else {
    return SoftLimit(x);
  }
  }