//    DaisySP is a DSP Library targeted at the Electrosmith Daisy Product Line.
//    Author: Stephen Hensley, 2019
//
//    However, this is decoupled from the hardware in such a way that it
//        should be useful outside of the ARM context with different build configurations.
//
//    A few general notes about the contents of the library:
//        - all memory usage is static.
//        - in cases of potentially large memory usage: the user will either supply a buffer and a size, or the class will be a template that can have size set at compile time.
//        - all modules will have an Init() function, and a Process() function.
//        - all modules, unless otherwise noted, will process a single sample at a time.
//        - all processing will be done with 'float' type unless otherwise noted.
//
#pragma once
#ifndef DSYSP_H
#define DSYSP_H

/** Control Modules */
#include "Control/adenv.h"
#include "Control/adsr.h"
#include "Control/phasor.h"

/** Drum Modules */
#include "Drums/analogbassdrum.h"
#include "Drums/analogsnaredrum.h"
#include "Drums/hihat.h"
#include "Drums/synthbassdrum.h"
#include "Drums/synthsnaredrum.h"

/** Dynamics Modules */
#include "Dynamics/crossfade.h"
#include "Dynamics/limiter.h"

/** Effects Modules */
#include "Effects/autowah.h"
#include "Effects/chorus.h"
#include "Effects/decimator.h"
#include "Effects/flanger.h"
#include "Effects/overdrive.h"
#include "Effects/pitchshifter.h"
#include "Effects/phaser.h"
#include "Effects/sampleratereducer.h"
#include "Effects/tremolo.h"
#include "Effects/wavefolder.h"

/** Filter Modules */
#include "Filters/onepole.h"
#include "Filters/svf.h"
#include "Filters/fir.h"
#include "Filters/soap.h"

/** Noise Modules */
#include "Noise/clockednoise.h"
#include "Noise/dust.h"
#include "Noise/fractal_noise.h"
#include "Noise/grainlet.h"
#include "Noise/particle.h"
#include "Noise/whitenoise.h"

/** Physical Modeling Modules */
#include "PhysicalModeling/drip.h"
#include "PhysicalModeling/KarplusString.h"
#include "PhysicalModeling/modalvoice.h"
#include "PhysicalModeling/resonator.h"
#include "PhysicalModeling/stringvoice.h"

/** Sampling Modules */
#include "Sampling/granularplayer.h"

/** Synthesis Modules */
#include "Synthesis/fm2.h"
#include "Synthesis/formantosc.h"
#include "Synthesis/harmonic_osc.h"
#include "Synthesis/oscillator.h"
#include "Synthesis/oscillatorbank.h"
#include "Synthesis/variablesawosc.h"
#include "Synthesis/variableshapeosc.h"
#include "Synthesis/vosim.h"
#include "Synthesis/zoscillator.h"

/** Utility Modules */
#include "Utility/dcblock.h"
#include "Utility/delayline.h"
#include "Utility/dsp.h"
#include "Utility/looper.h"
#include "Utility/maytrig.h"
#include "Utility/metro.h"
#include "Utility/samplehold.h"
#include "Utility/smooth_random.h"

/** LGPL Modules */
#ifdef USE_DAISYSP_LGPL
#include "daisysp-lgpl.h"
#endif

#endif
