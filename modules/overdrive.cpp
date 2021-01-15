#include "dsp.h"
#include "overdrive.h"
#include <algorithm>

using namespace daisysp;

void Overdrive::Init()
{
    SetDrive(.5f);
}

float Overdrive::Process(float in)
{
    float pre = pre_gain_ * in;
    return SoftClip(pre) * post_gain_;
}

void Overdrive::SetDrive(float drive)
{
    drive  = fclamp(drive, 0.f, 1.f);
    drive_ = 2.f * drive;

    const float drive_2    = drive_ * drive_;
    const float pre_gain_a = drive_ * 0.5f;
    const float pre_gain_b = drive_2 * drive_2 * drive_ * 24.0f;
    pre_gain_              = pre_gain_a + (pre_gain_b - pre_gain_a) * drive_2;

    const float drive_squashed = drive_ * (2.0f - drive_);
    post_gain_ = 1.0f / SoftClip(0.33f + drive_squashed * (pre_gain_ - 0.33f));
}

inline float Overdrive::SoftLimit(float x)
{
    return x * (27.0f + x * x) / (27.0f + 9.0f * x * x);
}

inline float Overdrive::SoftClip(float x)
{
    if(x < -3.0f)
    {
        return -1.0f;
    }
    else if(x > 3.0f)
    {
        return 1.0f;
    }
    else
    {
        return SoftLimit(x);
    }
}