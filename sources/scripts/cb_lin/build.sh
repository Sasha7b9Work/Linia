#!/usr/bin/env bash

dir=$PWD
cd ../../generated
cmake --build . -- -j4
cd $dir

