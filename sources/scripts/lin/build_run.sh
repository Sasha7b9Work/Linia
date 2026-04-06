#!/usr/bin/env bash

#set -e

./build.sh
kill -9 $(pidof Linia2)

export DISPLAY=:0
/home/mnipi/Desktop/Linia.desktop

#./start_x.sh
