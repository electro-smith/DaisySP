cmake_minimum_required(VERSION 3.14)

project(DAISYSP VERSION 0.0.1)

add_library(DaisySP STATIC 
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
Source/Effects/phaser.cpp
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


set_target_properties(DaisySP PROPERTIES PUBLIC
  CXX_STANDARD 14 
  CXX_STANDARD_REQUIRED
  )


target_include_directories(DaisySP PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/Source
  PRIVATE
  "Source"
  "Source/Control"
  "Source/Drums"
  "Source/Dynamics"
  "Source/Effects"
  "Source/Filters"
  "Source/Noise"
  "Source/PhysicalModeling"
  "Source/Synthesis"
  "Source/Utility"
  )
