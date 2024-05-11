#!/bin/bash

if [ "$EUID" -ne 0 ]
  then echo "please run as root"
  exit
fi

xbps-install -Syu ffmpeg-devel taglib-devel ffmpeg taglib qt6-multimedia qt6-multimedia-devel qt6-core qt6-base qt6-base-devel qt6-widgets qt6-gui
