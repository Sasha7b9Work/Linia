#!/usr/bin/env bash

cd ../wxWidgets
rm -R -f generated
cd ../linux
cmake ../../../ThirdParty/wxWidgets/CMakeLists.txt -B../../../generated/wxWidgets -G "CodeBlocks - Unix Makefiles" -DwxBUILD_SAMPLES=OFF -DwxBUILD_SHARED=OFF -DwxBUILD_STRIPPED_RELEASE=ON -DCMAKE_CXX_FLAGS_INIT="-pipe" -DCMAKE_C_FLAGS_INIT="-pipe"
