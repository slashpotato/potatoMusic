#!/bin/bash
make install --jobs=$(grep -c ^processor /proc/cpuinfo)
