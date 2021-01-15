#include "dsp.h"
#include "diffuser.h"
#include <math.h>

using namespace daisysp;

void Diffuser::Init()
{
    engine_.Init(buffer_);
    engine_.SetLFOFrequency(LFO_1, 0.3f / 48000.0f);
    lp_decay_ = 0.0f;
}

float Diffuser::Process(float amount, float rt, float in)
{
    typedef E::Reserve<
        126,
        E::Reserve<
            180,
            E::Reserve<
                269,
                E::Reserve<
                    444,
                    E::Reserve<1653, E::Reserve<2010, E::Reserve<3411>>>>>>>
                            Memory;
    E::DelayLine<Memory, 0> ap1;
    E::DelayLine<Memory, 1> ap2;
    E::DelayLine<Memory, 2> ap3;
    E::DelayLine<Memory, 3> ap4;
    E::DelayLine<Memory, 4> dapa;
    E::DelayLine<Memory, 5> dapb;
    E::DelayLine<Memory, 6> del;
    E::Context              c;
    const float             kap = 0.625f;
    const float             klp = 0.75f;
    float                   lp  = lp_decay_;

    float wet;
    engine_.Start(&c);
    c.Read(in);
    c.Read(ap1 TAIL, kap);
    c.WriteAllPass(ap1, -kap);
    c.Read(ap2 TAIL, kap);
    c.WriteAllPass(ap2, -kap);
    c.Read(ap3 TAIL, kap);
    c.WriteAllPass(ap3, -kap);
    c.Interpolate(ap4, 400.0f, LFO_1, 43.0f, kap);
    c.WriteAllPass(ap4, -kap);
    c.Interpolate(del, 3070.0f, LFO_1, 340.0f, rt);
    c.Lp(lp, klp);
    c.Read(dapa TAIL, -kap);
    c.WriteAllPass(dapa, kap);
    c.Read(dapb TAIL, kap);
    c.WriteAllPass(dapb, -kap);
    c.Write(del, 2.0f);
    c.Write(wet, 0.0f);

    lp_decay_ = lp;

    return .1f * amount * (wet - in);
}
