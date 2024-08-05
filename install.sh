#!/bin/bash

if [ "$EUID" -ne 0 ]
  then echo "Please run as root."
  exit
fi

if [ -f /bin/xbps-install ]; then
        xbps-install -yu ffmpeg-devel taglib-devel ffmpeg taglib qt6-multimedia qt6-multimedia-devel qt6-core qt6-base qt6-base-devel qt6-widgets qt6-gui &> /dev/null
elif [ -f /bin/apt ]; then
        apt install -y qt6-multimedia-dev qt6-base-dev ffmpeg libtag1-dev &> /dev/null
else
        echo "Plese install dependencies by yourself. Your package manager is not supported."
fi

qmake6 || qmake-qt6 || /lib/qt6/bin/qmake6
make install --jobs=$(grep -c ^processor /proc/cpuinfo) &> /dev/null && echo "Successfully installed" || echo "Error"
cp *.desktop /usr/local/share/applications && echo "Created desktop entry"
