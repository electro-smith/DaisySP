//    DaisySP is a DSP Library targeted at the Electrosmith Daisy Product Line.
//  Author: Stephen Hensley, 2019
//
//    However, this is decoupled from the hardware in such a way that it
//        should be useful outside of the ARM context with different build configurations.
//
//    A few general notes about the contents of the library:
//        - all memory usage is static.
//        - in cases of potentially large memory usage: the user will either supply a buffer and a size, or the class will be a template that can have size set at compile time.
//         - all modules will have an Init() function, and a Process() function.
//        - all modules, unless otherwise noted, will process a single sample at a time.
//        - all processing will be done with 'float' type unless otherwise noted.
//
#ifndef DSYSP_H
#define DSYSP_H

// Listed in Alphabetical Order
// Each of the below has/should have an example project.
#include "modules/Control/adenv.h"
#include "modules/Control/adsr.h"
#include "modules/Filters/allpass.h"
#include "modules/Drums/analogbassdrum.h"
#include "modules/Drums/analogsnaredrum.h"
#include "modules/Filters/atone.h"
#include "modules/Effects/autowah.h"
#include "modules/Dynamics/balance.h"
#include "modules/Filters/biquad.h"
#include "modules/Synthesis/blosc.h"
#include "modules/Filters/comb.h"
#include "modules/Effects/bitcrush.h"
#include "modules/Noise/clockednoise.h"
#include "modules/Dynamics/compressor.h"
#include "modules/Dynamics/crossfade.h"
#include "modules/Utility/dcblock.h"
#include "modules/Effects/decimator.h"
#include "modules/Utility/delayline.h"
#include "modules/PhysicalModeling/drip.h"
#include "modules/Utility/dsp.h"
#include "modules/Noise/dust.h"
#include "modules/Synthesis/fm2.h"
#include "modules/Effects/fold.h"
#include "modules/Synthesis/formantosc.h"
#include "modules/Noise/fractal_noise.h"
#include "modules/Noise/grainlet.h"
#include "modules/Synthesis/harmonic_osc.h"
#include "modules/Drums/hihat.h"
#include "modules/Utility/jitter.h"
#include "modules/Dynamics/limiter.h"
#include "modules/Control/line.h"
#include "modules/Utility/maytrig.h"
#include "modules/Utility/metro.h"
#include "modules/Filters/mode.h"
#include "modules/Filters/moogladder.h"
#include "modules/Filters/nlfilt.h"
#include "modules/Synthesis/oscillator.h"
#include "modules/Synthesis/oscillatorbank.h"
#include "modules/Effects/overdrive.h"
#include "modules/Noise/particle.h"
#include "modules/Synthesis/phasor.h"
#include "modules/Utility/port.h"
#include "modules/Effects/pitchshifter.h"
#include "modules/PhysicalModeling/pluck.h"
#include "modules/PhysicalModeling/PolyPluck.h"
#include "modules/PhysicalModeling/resonator.h"
#include "modules/Effects/reverbsc.h"
#include "modules/Utility/samplehold.h"
#include "modules/Effects/sampleratereducer.h"
#include "modules/Utility/smooth_random.h"
#include "modules/PhysicalModeling/string.h"
#include "modules/Filters/svf.h"
#include "modules/Drums/synthbassdrum.h"
#include "modules/Drums/synthsnaredrum.h"
#include "modules/Filters/tone.h"
#include "modules/Synthesis/variablesawosc.h"
#include "modules/Synthesis/variableshapeosc.h"
#include "modules/Synthesis/vosim.h"
#include "modules/Noise/whitenoise.h"
#include "modules/Synthesis/zoscillator.h"

#endif
