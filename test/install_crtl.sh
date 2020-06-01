#!/bin/bash

rm -rf /usr/local/include/crtl/*

cd ../build
cmake ..
make && make install
cd -
