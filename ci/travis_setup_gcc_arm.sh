#!/bin/bash

# setup script to install texlive and add to path
sudo apt-get -qq update
export PATH=/tmp/gcc-arm/bin/:$PATH


echo "Downloading gcc-arm-none-eabi"
wget "https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2020q2/gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz2"
tar -xf "gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz2"
ln -s "$(pwd)/gcc-arm-none-eabi-9-2020-q2-update" "/tmp/gcc-arm"

echo "Finished extracting gcc-arm-none-eabi"
