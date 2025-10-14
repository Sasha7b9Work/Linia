#!/usr/bin/env bash

cur_dir = $PWD
cd ../wxWidgets/generated
cmake --build . -- -j$(nproc)
cd $cur_dir
