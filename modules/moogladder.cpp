#include "moogladder.h"
#include <math.h>

using namespace daisysp;

float MoogLadder::my_tanh(float x)
{
    int sign = 1;
    if (x < 0)
    {
        sign = -1;
	x = -x;
	return x * sign;
    }
    else if (x >= 4.0)
    {
        return sign;
    }
    else if (x < 0.5)
    {
        return x * sign;
    }
    return sign * tanhf(x);
}

void MoogLadder::Init(float sample_rate)
{
    sample_rate_ = sample_rate;
    istor_ = 0.0;
    res_ = 0.4;
    freq_ = 1000;
    
    for (int i = 0; i < 6; i++)
    {
        delay_[i] = 0.0;
	tanhstg_[i%3] = 0.0;
    }

    old_freq_ = 0.0;
    old_res_ = -1.0;
}

float MoogLadder::Process(float in)
{
    float freq = freq_;
    float res = res_;
    float res4;
    float* delay = delay_;
    float* tanhstg = tanhstg_;
    float stg[4];
    float acr, tune;
    
    float THERMAL = 0.000025;

    if (res < 0) { res = 0; }

    if (old_freq_ != freq || old_res_ != res)
    {
        float f, fc, fc2, fc3, fcr;
	old_freq_ = freq;
	fc = (freq / sample_rate_);
	f = 0.5 * fc;
	fc2 = fc * fc;
	fc3 = fc2 * fc2;

        fcr = 1.8730*fc3 + 0.4955*fc2 - 0.6490*fc + 0.9988;
        acr = -3.9364*fc2 + 1.8409*fc + 0.9968;
        tune = (1.0 - exp(-((2 * M_PI)*f*fcr))) / THERMAL;

	old_res_ = res;
	old_acr_ = acr;
	old_tune_ = tune;
    }
    else
    {
        res = old_res_;
	acr = old_acr_;
	tune = old_tune_;
    }

    res4 = 4.0f * res * acr;

    for (int j = 0; j < 2; j++)
    {
        in -= res4 * delay[5];
	delay[0] = stg[0] = delay[0] + tune * (my_tanh(in * THERMAL) - tanhstg[0]);
	for (int k = 1; k < 4; k++)
	{
  	    in = stg[k-1];
	    stg[k] = delay[k]
            + tune*((tanhstg[k-1] = my_tanh(in*THERMAL))
                    - (k != 3 ? tanhstg[k] : my_tanh(delay[k]*THERMAL)));
	    delay[k] = stg[k];
	}
	delay[5] = (stg[3] + delay[4]) * 0.5f;
	delay[4] = stg[3];
    }
    return delay[5];
}
