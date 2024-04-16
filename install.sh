#!/bin/bash
./build.sh
make install --jobs=$(grep -c ^processor /proc/cpuinfo)
