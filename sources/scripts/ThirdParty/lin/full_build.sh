#!/usr/bin/env bash

dir=$PWD
cd ../../..
rm -R -f generated
cd $dir

cmake ../../../ThirdParty/wxWidgets/CMakeLists.txt -B../../../generated/ThirdParty -G "CodeBlocks - Unix Makefiles" -DwxBUILD_SAMPLES=OFF -DwxBUILD_SHARED=OFF

cd ../../../generated/ThirdParty
cmake --build . -- -j$(nproc)
cd $dir
