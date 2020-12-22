#!/bin/bash
#TODO:
# - fix all the hard-coding
STARTDIR=`pwd`
find $STARTDIR -iname '*.h' -type f -exec sed -i 's/\t/    /g' {} +
find $STARTDIR -iname '*.cpp' -type f -exec sed -i 's/\t/    /g' {} +
clang-format -i $STARTDIR/modules/*.cpp $STARTDIR/modules/*.h
clang-format -i $STARTDIR/examples/*/*.cpp

