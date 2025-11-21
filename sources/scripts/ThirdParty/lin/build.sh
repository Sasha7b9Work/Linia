#!/usr/bin/env bash

cd ../../generated/wxWidgets/
cmake --build . -- -j$(nproc)
