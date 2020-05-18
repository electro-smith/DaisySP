#!/bin/bash
start_dir=$PWD

echo
echo "Rebuild..."
echo

make
cd build/
dfu-tool reset
dfu-tool convert dfuse *.hex ex.dfu
dfu-util -a 0 -D ex.dfu

echo
echo "Success!!!"
echo
