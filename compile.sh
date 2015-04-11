#!/bin/bash
exec > /dev/null
mkdir -p build
pushd build
c++ ../linux_handmade.cpp -o handmadehero -g `sdl2-config --cflags --libs`
popd
