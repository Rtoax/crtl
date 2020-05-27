#!/bin/bash

cd ../build
cmake ..
make && make install
cd -
