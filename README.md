# potatoMusic
An qt music player for linux

![GitHub License](https://img.shields.io/github/license/slashpotato/potatoMusic?style=flat-square&labelColor=black&color=white%20)
![GitHub Release](https://img.shields.io/github/v/release/slashpotato/potatoMusic?display_name=release&style=flat-square&labelColor=black&color=white)

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
This software is dual-licensed under the GPL-3.0 and the BSD-3-Clause licenses, and also available under the CC-BY-NC-SA-4.0 license for third parties
You can choose between one of them if you use this work

`SPDX-License-Identifier: GPL-3.0 OR CC-BY-NC-SA-4.0`
