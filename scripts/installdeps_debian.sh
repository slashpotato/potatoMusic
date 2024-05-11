#!/bin/bash

if [ "$EUID" -ne 0 ]
  then echo "please run as root"
  exit
fi

apt install qt6-multimedia-dev qt6-base-dev ffmpeg libtag1-dev -y
