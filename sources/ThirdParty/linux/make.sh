#!/usr/bin/env bash

cd ../wxWidgets
rm -R -f generated
cd ../linux
cmake ../wxWidgets/CMakeLists.txt -B../wxWidgets/generated -G "CodeBlocks - Unix Makefiles" -DwxBUILD_SAMPLES=OFF -DwxBUILD_SHARED=OFF -DwxBUILD_STRIPPED_RELEASE=ON -DCMAKE_CXX_FLAGS="-pipe" -DCMAKE_C_FLAGS="-pipe"
