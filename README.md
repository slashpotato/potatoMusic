# potatoMusic
An qt music player for linux

![GitHub License](https://img.shields.io/github/license/slashpotato/potatoMusic?style=flat-square&labelColor=black&color=white)
![GitHub Release](https://img.shields.io/github/v/release/slashpotato/potatoMusic?display_name=tag&style=flat-square&labelColor=black&color=white&logo=qt&logoColor=white)

![screenshot1](https://github.com/slashpotato/potatoMusic/blob/master/screenshot1.png?raw=true)

theme used on screenshot: [catppuccin mocha lavender](https://github.com/catppuccin/kde)

icons user on screenshot: [papirus](https://github.com/PapirusDevelopmentTeam/papirus-icon-theme) (recommended)
## usage
### dependencies 
- qt6-base
- qt6-multimedia
- taglib
- ffmpeg
### build
Run `build.sh`

#### **or**
```shell
qmake6
make --jobs=$(grep -c ^processor /proc/cpuinfo)
```
### run
Run `./potatoMusic` after building
### install
Run `make install` afrer building to install. `make uninstall` to uninstall
## license
This software is licensed under GPL-3.0

`SPDX-License-Identifier: GPL-3.0`
