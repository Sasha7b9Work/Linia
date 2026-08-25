#!/usr/bin/env bash

#set -e

./build.sh
kill $(pidof Linia2)

./start_x.sh
