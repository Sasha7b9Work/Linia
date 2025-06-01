#!/usr/bin/env bash

dir=$PWD
cd ../../generated
cmake --build . -- -j$(nproc)
cd $dir

