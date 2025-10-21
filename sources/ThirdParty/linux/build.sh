#!/usr/bin/env bash

cd ../wxWidgets/generated
cmake --build . -- -j$(nproc)
