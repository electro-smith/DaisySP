
cmake_minimum_required(VERSION 3.14)

project(DAISYSP VERSION 0.0.1)

set(SOURCE_FILES 
Source/Control/adenv.cpp
Source/Control/adsr.cpp
Source/Control/line.cpp
Source/Control/phasor.cpp
Source/Drums/analogbassdrum.cpp
Source/Drums/analogsnaredrum.cpp
Source/Drums/hihat.cpp
Source/Drums/synthbassdrum.cpp
Source/Drums/synthsnaredrum.cpp
Source/Dynamics/balance.cpp
Source/Dynamics/compressor.cpp
Source/Dynamics/crossfade.cpp
Source/Dynamics/limiter.cpp
Source/Effects/autowah.cpp
Source/Effects/bitcrush.cpp
Source/Effects/chorus.cpp
Source/Effects/decimator.cpp
Source/Effects/flanger.cpp
Source/Effects/fold.cpp
Source/Effects/overdrive.cpp
Source/Effects/reverbsc.cpp
Source/Effects/sampleratereducer.cpp
Source/Effects/tremolo.cpp
Source/Filters/allpass.cpp
Source/Filters/atone.cpp
Source/Filters/biquad.cpp
Source/Filters/comb.cpp
Source/Filters/mode.cpp
Source/Filters/moogladder.cpp
Source/Filters/nlfilt.cpp
Source/Filters/svf.cpp
Source/Filters/tone.cpp
Source/Noise/clockednoise.cpp
Source/Noise/grainlet.cpp
Source/Noise/particle.cpp
Source/PhysicalModeling/drip.cpp
Source/PhysicalModeling/modalvoice.cpp
Source/PhysicalModeling/pluck.cpp
Source/PhysicalModeling/resonator.cpp
Source/PhysicalModeling/KarplusString.cpp
Source/PhysicalModeling/stringvoice.cpp
Source/Synthesis/blosc.cpp
Source/Synthesis/fm2.cpp
Source/Synthesis/formantosc.cpp
Source/Synthesis/oscillator.cpp
Source/Synthesis/oscillatorbank.cpp
Source/Synthesis/variablesawosc.cpp
Source/Synthesis/variableshapeosc.cpp
Source/Synthesis/vosim.cpp
Source/Synthesis/zoscillator.cpp
Source/Utility/dcblock.cpp
Source/Utility/jitter.cpp
Source/Utility/metro.cpp
Source/Utility/port.cpp
)

set( HEADER_FILES
Source/Control/adenv.h
Source/Control/adsr.h
Source/Control/line.h
Source/Control/phasor.h
Source/Drums/analogbassdrum.h
Source/Drums/analogsnaredrum.h
Source/Drums/hihat.h
Source/Drums/synthbassdrum.h
Source/Drums/synthsnaredrum.h
Source/Dynamics/balance.h
Source/Dynamics/compressor.h
Source/Dynamics/crossfade.h
Source/Dynamics/limiter.h
Source/Effects/autowah.h
Source/Effects/bitcrush.h
Source/Effects/chorus.h
Source/Effects/decimator.h
Source/Effects/flanger.h
Source/Effects/fold.h
Source/Effects/overdrive.h
Source/Effects/reverbsc.h
Source/Effects/pitchshifter.h
Source/Effects/sampleratereducer.h
Source/Effects/tremolo.h
Source/Filters/allpass.h
Source/Filters/atone.h
Source/Filters/biquad.h
Source/Filters/comb.h
Source/Filters/mode.h
Source/Filters/moogladder.h
Source/Filters/nlfilt.h
Source/Filters/svf.h
Source/Filters/tone.h
Source/Noise/clockednoise.h
Source/Noise/dust.h
Source/Noise/fractal_noise.h
Source/Noise/grainlet.h
Source/Noise/particle.h
Source/Noise/whitenoise.h
Source/PhysicalModeling/drip.h
Source/PhysicalModeling/modalvoice.h
Source/PhysicalModeling/pluck.h
Source/PhysicalModeling/PolyPluck.h
Source/PhysicalModeling/resonator.h
Source/PhysicalModeling/KarplusString.h
Source/PhysicalModeling/stringvoice.h
Source/Synthesis/blosc.h
Source/Synthesis/fm2.h
Source/Synthesis/formantosc.h
Source/Synthesis/harmonic_osc.h
Source/Synthesis/oscillator.h
Source/Synthesis/oscillatorbank.h
Source/Synthesis/variablesawosc.h
Source/Synthesis/variableshapeosc.h
Source/Synthesis/vosim.h
Source/Synthesis/zoscillator.h
Source/Utility/dcblock.h
Source/Utility/delayline.h
Source/Utility/dsp.h
Source/Utility/jitter.h
Source/Utility/maytrig.h
Source/Utility/metro.h
Source/Utility/port.h
Source/Utility/samplehold.h
Source/Utility/smooth_random.h
)

include_directories(Source)
include_directories(Source/Control)
include_directories(Source/Drums)
include_directories(Source/Dynamics)
include_directories(Source/Effects)
include_directories(Source/Filters)
include_directories(Source/Noise)
include_directories(Source/PhysicalModeling)
include_directories(Source/Synthesis)
include_directories(Source/Utility)

set(CMAKE_CXX_STANDARD 14)

ADD_LIBRARY( DaisySP STATIC 
    ${SOURCE_FILES} 
    ${HEADER_FILES} 
    Source/daisysp.h )
TARGET_INCLUDE_DIRECTORIES(DaisySP PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Source>" "$<INSTALL_INTERFACE:Source>")

