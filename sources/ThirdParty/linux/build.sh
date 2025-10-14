#!/usr/bin/env bash

dir = $PWD
cd ../wxWidgets/generated
cmake --build . -- -j$(nproc)
cd $dir
