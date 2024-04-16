#!/bin/bash
make uninstall --jobs=$(grep -c ^processor /proc/cpuinfo)
