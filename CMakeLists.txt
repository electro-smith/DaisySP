
cmake_minimum_required(VERSION 3.15)

project(DAISYSP VERSION 0.0.1)

file(GLOB_RECURSE SOURCE_FILES modules/*.cpp)
file(GLOB_RECURSE HEADER_FILES modules/*.h)

include_directories(modules/Control)
include_directories(modules/Drums)
include_directories(modules/Dynamics)
include_directories(modules/Effects)
include_directories(modules/Filters)
include_directories(modules/Noise)
include_directories(modules/PhysicalModeling)
include_directories(modules/Synthesis)
include_directories(modules/Utility)

ADD_LIBRARY( DaisySP STATIC 
    ${SOURCE_FILES} 
    ${HEADER_FILES} 
    daisysp.h )

