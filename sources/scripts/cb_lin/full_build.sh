#!/usr/bin/env bash

rm -R -f ../../generated

cmake ../../CMakeLists.txt -B../../generated -G "CodeBlocks - Unix Makefiles"

./build.sh
