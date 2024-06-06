#!/bin/bash

if [ "$EUID" -ne 0 ]
  then echo "Please run as root."
  exit
fi

make uninstall --jobs=$(grep -c ^processor /proc/cpuinfo)
