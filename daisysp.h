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
#include "modules/adenv.h"
#include "modules/adsr.h"
#include "modules/allpass.h"
#include "modules/analogsnaredrum.h"
#include "modules/atone.h"
#include "modules/autowah.h"
#include "modules/balance.h"
#include "modules/biquad.h"
#include "modules/blosc.h"
#include "modules/comb.h"
#include "modules/bitcrush.h"
#include "modules/compressor.h"
#include "modules/crossfade.h"
#include "modules/dcblock.h"
#include "modules/decimator.h"
#include "modules/delayline.h"
#include "modules/drip.h"
#include "modules/dsp.h"
#include "modules/fm2.h"
#include "modules/fold.h"
#include "modules/formantosc.h"
#include "modules/grainlet.h"
#include "modules/harmonic_osc.h"
#include "modules/jitter.h"
#include "modules/limiter.h"
#include "modules/line.h"
#include "modules/maytrig.h"
#include "modules/metro.h"
#include "modules/mode.h"
#include "modules/moogladder.h"
#include "modules/nlfilt.h"
#include "modules/oscillator.h"
#include "modules/oscillatorbank.h"
#include "modules/phasor.h"
#include "modules/port.h"
#include "modules/pitchshifter.h"
#include "modules/PolyPluck.h"
#include "modules/pluck.h"
#include "modules/reverbsc.h"
#include "modules/samplehold.h"
#include "modules/svf.h"
#include "modules/tone.h"
#include "modules/variablesawosc.h"
#include "modules/variableshapeosc.h"
#include "modules/vosim.h"
#include "modules/whitenoise.h"
#include "modules/zoscillator.h"

#endif
