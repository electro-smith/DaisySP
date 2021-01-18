
cmake_minimum_required(VERSION 3.15)

project(DAISYSP VERSION 0.0.1)

file(GLOB SOURCE_FILES modules/*.cpp)
file(GLOB HEADER_FILES modules/*.h)

include_directories(modules)

ADD_LIBRARY( DaisySP STATIC 
    ${SOURCE_FILES} 
    ${HEADER_FILES} 
    daisysp.h )

