//	DaisySP is a DSP Library targeted at the Electrosmith Daisy Product Line.
//  Author: Stephen Hensley, 2019
//
//	However, this is decoupled from the hardware in such a way that it
//		should be useful outside of the ARM context with different build configurations.
//
//	A few general notes about the contents of the library:
// 		- all types will end with '_t'
//		- all memory usage is static.
//		- in cases of potentially large memory usage, the user will supply a buffer and a size.
// 		- all blocks will have an _init() function, and a _process() function.
//		- all blocks, unless otherwise noted, will process a single sample at a time.
//		- all processing will be done with 'float' type unless otherwise noted.
//		- a common dsysp_data_t type will be used for many init() functions to reduce user code complexity
//
#ifndef DSYSP_H
#define DSYSP_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	float samplerate;
	float channels;
}dsysp_data_t;

#define DSY_MIN(in, mn) (in < mn ? in : mn)
#define DSY_MAX(in, mx) (in > mx ? in : mx)
#define DSY_CLAMP(in, mn, mx) (DSY_MIN(DSY_MAX(in, mn), mx))

// Listed in Alphabetical Order
// Each of the below has/should have an example project.
#include "modules/adenv.h"
#include "modules/crossfade.h"
#include "modules/dcblock.h"
#include "modules/decimator.h"
#include "modules/line.h"
#include "modules/nlfilt.h"
#include "modules/metro.h"
#include "modules/phasor.h"
#include "modules/port.h"
#include "modules/tone.h"
  
#ifdef __cplusplus
}

#include "modules/dsp.h"
#include "modules/limiter.h"
#include "modules/mode.h"
#include "modules/delayline.h"
#include "modules/reverbsc.h"
#include "modules/whitenoise.h"
#include "modules/svf.h"
#include "modules/oscillator.h"
#include "modules/compressor.h"
#include "modules/pitchshifter.h"
#include "modules/pluck.h"
#include "modules/PolyPluck.h"

#endif
#endif
