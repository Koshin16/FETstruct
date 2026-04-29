#!/bin/bash
set -e

sudo apt update
sudo apt install -y build-essential freeglut3-dev mesa-common-dev wget

if [ ! -f include/stb_image_write.h ]; then
    wget -O include/stb_image_write.h https://raw.githubusercontent.com/nothings/stb/master/stb_image_write.h
fi

make clean
make
./ofet
