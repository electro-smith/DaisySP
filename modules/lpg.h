#include <algorithm>

//#include "stmlib/dsp/dsp.h"
//#include "stmlib/dsp/filter.h"
//#include "stmlib/dsp/parameter_interpolator.h"

#pragma once
#ifndef DSY_LOWPASSGATE_H
#define DSY_LOWPASSGATE_H

#include <stdint.h>
#include "Svf.h"
#ifdef __cplusplus

/** @file lpg.h */

namespace daisysp
{
/**    
       @brief Approximate Lowpass Gate \n
	   @author Ported by Ben Sergentanis 
	   @date Jan 2021 
	   Ported from pichenettes/eurorack/plaits/dsp/fx/low_pass_gate.h \n
	   to an independent module. \n
	   Original code written by Emilie Gillet in 2016. \n
*/
class LowPassGate
{
  public:
    LowPassGate() {}
    ~LowPassGate() {}

    void Init(float sample_rate);

    float Process(float gain, float frequency, float hf_bleed, float in);

    //this is the one that actually gets used in plaits
    //seems like stride always just defaults to 2
    float Process(float  gain,
                  float  frequency,
                  float  hf_bleed,
                  size_t stride,
                  float  in);

  private:
    Svf   filter_;
    float sample_rate_;

    int Clip16(int input) { return std::min(std::max(0, input), 65535); };
};
} // namespace daisysp
#endif
#endif