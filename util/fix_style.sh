#!/bin/bash
#TODO:
# - fix all the hard-coding
STARTDIR=`pwd`
find $STARTDIR -iname '*.h' -type f -exec sed -i 's/\t/    /g' {} +
find $STARTDIR -iname '*.cpp' -type f -exec sed -i 's/\t/    /g' {} +
clang-format-6.0 -i $STARTDIR/modules/*.cpp $STARTDIR/modules/*.h
clang-format-6.0 -i $STARTDIR/examples/*/*.cpp 
