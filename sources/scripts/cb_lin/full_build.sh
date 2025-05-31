#!/usr/bin/env bash

rm -R -f ../../generated

cmake ../../VS/CMakeLists.txt -B../../generated -G "CodeBlocks - Unix Makefiles"

./build.sh
