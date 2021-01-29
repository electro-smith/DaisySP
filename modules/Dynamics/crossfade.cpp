#include <math.h>
#include "crossfade.h"
#include "dsp.h"

#define REALLYSMALLFLOAT 0.000001f

using namespace daisysp;

const float kCrossLogMin = logf(REALLYSMALLFLOAT);
const float kCrossLogMax = logf(1.0f);

float CrossFade::Process(float &in1, float &in2)
{
    float scalar_1, scalar_2;
    switch(curve_)
    {
        case CROSSFADE_LIN:
            scalar_1 = pos_;
            return (in1 * (1.0f - scalar_1)) + (in2 * scalar_1);

        case CROSSFADE_CPOW:
            scalar_1 = sinf(pos_ * HALFPI_F);
            scalar_2 = sinf((1.0f - pos_) * HALFPI_F);
            return (in1 * scalar_2) + (in2 * scalar_1);

        case CROSSFADE_LOG:
            scalar_1
                = expf(pos_ * (kCrossLogMax - kCrossLogMin) + kCrossLogMin);
            return (in1 * (1.0f - scalar_1)) + (in2 * scalar_1);

        case CROSSFADE_EXP:
            scalar_1 = pos_ * pos_;
            return (in1 * (1.0f - scalar_1)) + (in2 * scalar_1);

        default: return 0;
    }
}
