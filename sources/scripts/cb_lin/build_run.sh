#!/usr/bin/env bash

#set -e

kill -9 $(pidof Linia2)
git pull
./build.sh
