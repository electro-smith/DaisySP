
cmake_minimum_required(VERSION 3.15)

project(DAISYSP VERSION 0.0.1)

file(GLOB_RECURSE SOURCE_FILES modules/*.cpp)
file(GLOB_RECURSE HEADER_FILES modules/*.h)

include_directories(modules)

ADD_LIBRARY( DaisySP STATIC 
    ${SOURCE_FILES} 
    ${HEADER_FILES} 
    daisysp.h )

