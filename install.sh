#!/bin/bash

if [ "$EUID" -ne 0 ]
  then echo "Please run as root."
  exit
fi

make install --jobs=$(grep -c ^processor /proc/cpuinfo)
