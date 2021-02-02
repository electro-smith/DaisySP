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
#include "adenv.h"
#include "adsr.h"
#include "allpass.h"
#include "analogbassdrum.h"
#include "analogsnaredrum.h"
#include "atone.h"
#include "autowah.h"
#include "balance.h"
#include "biquad.h"
#include "blosc.h"
#include "comb.h"
#include "bitcrush.h"
#include "clockednoise.h"
#include "compressor.h"
#include "crossfade.h"
#include "dcblock.h"
#include "decimator.h"
#include "delayline.h"
#include "drip.h"
#include "dsp.h"
#include "dust.h"
#include "fm2.h"
#include "fold.h"
#include "formantosc.h"
#include "fractal_noise.h"
#include "grainlet.h"
#include "harmonic_osc.h"
#include "hihat.h"
#include "jitter.h"
#include "limiter.h"
#include "line.h"
#include "maytrig.h"
#include "metro.h"
#include "modalvoice.h"
#include "mode.h"
#include "moogladder.h"
#include "nlfilt.h"
#include "oscillator.h"
#include "oscillatorbank.h"
#include "overdrive.h"
#include "particle.h"
#include "phasor.h"
#include "port.h"
#include "pitchshifter.h"
#include "pluck.h"
#include "PolyPluck.h"
#include "resonator.h"
#include "reverbsc.h"
#include "samplehold.h"
#include "sampleratereducer.h"
#include "smooth_random.h"
#include "string.h"
#include "stringvoice.h"
#include "svf.h"
#include "synthbassdrum.h"
#include "synthsnaredrum.h"
#include "tone.h"
#include "tremolo.h"
#include "variablesawosc.h"
#include "variableshapeosc.h"
#include "vosim.h"
#include "whitenoise.h"
#include "zoscillator.h"

#endif
