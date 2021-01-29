#include "autowah.h"
#include <math.h>

using namespace daisysp;


void Autowah::Init(float sample_rate)
{
    sampling_freq_ = sample_rate;
    const1_        = 1413.72f / sampling_freq_;
    const2_        = expf(0.0f - (100.0f / sampling_freq_));
    const4_        = expf(0.0f - (10.0f / sampling_freq_));

    wet_dry_ = 100.0f;
    level_   = 0.1f;
    wah_     = 0.0;


    for(int i = 0; i < 2; i++)
    {
        rec1_[i] = rec2_[i] = rec3_[i] = rec4_[i] = rec5_[i] = 0.0f;
    }

    for(int i = 0; i < 3; i++)
    {
        rec0_[i] = 0.0f;
    }
}

float Autowah::Process(float in)
{
    float out;
    float fSlow2 = (0.01f * (wet_dry_ * level_));
    float fSlow3 = (1.0f - 0.01f * wet_dry_) + (1.f - wah_);

    float fTemp1 = fabs(in);
    rec3_[0]
        = fmaxf(fTemp1, (const4_ * rec3_[1]) + ((1.0f - const4_) * fTemp1));
    rec2_[0]     = (const2_ * rec2_[1]) + ((1.0f - const2_) * rec3_[0]);
    float fTemp2 = fminf(1.0f, rec2_[0]);
    float fTemp3 = powf(2.0f, (2.3f * fTemp2));
    float fTemp4
        = 1.0f
          - (const1_ * fTemp3 / powf(2.0f, (1.0f + 2.0f * (1.0f - fTemp2))));
    rec1_[0]
        = ((0.999f * rec1_[1])
           + (0.001f
              * (0.0f - (2.0f * (fTemp4 * cosf((const1_ * 2 * fTemp3)))))));
    rec4_[0] = ((0.999f * rec4_[1]) + (0.001f * fTemp4 * fTemp4));
    rec5_[0] = ((0.999f * rec5_[1]) + (0.0001f * powf(4.0f, fTemp2)));
    rec0_[0] = (0.0f
                - (((rec1_[0] * rec0_[1]) + (rec4_[0] * rec0_[2]))
                   - (fSlow2 * (rec5_[0] * in))));

    out      = ((wah_ * (rec0_[0] - rec0_[1])) + (fSlow3 * in));
    rec3_[1] = rec3_[0];
    rec2_[1] = rec2_[0];
    rec1_[1] = rec1_[0];
    rec4_[1] = rec4_[0];
    rec5_[1] = rec5_[0];
    rec0_[2] = rec0_[1];
    rec0_[1] = rec0_[0];

    return out;
}
