#!/usr/bin/env bash

git pull
dir=$PWD
cd ../../generated
cmake --build . -- -j$(nproc)
cd $dir

