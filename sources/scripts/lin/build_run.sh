#!/usr/bin/env bash

#set -e

./build.sh
kill -9 $(pidof Linia2)

exec ~/Linia.sh

#./start_x.sh
