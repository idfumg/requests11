#!/usr/bin/sh

mkdir build
cd build
cmake ..
make -j4
cp -r ../test/cert test/
./test/tests
