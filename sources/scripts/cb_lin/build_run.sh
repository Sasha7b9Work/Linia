#!/usr/bin/env bash

#set -e

git pull
./build.sh
kill -9 $(pidof Linia2)

#./start_x.sh
