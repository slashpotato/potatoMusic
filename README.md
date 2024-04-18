# potatoMusic
<marquee>An qt music player for linux </marquee>

![screenshot1](https://github.com/slashpotato/potatoMusic/blob/master/screenshot1.png?raw=true)

theme used on screenshot: [catppuccin mocha lavender](https://github.com/catppuccin/kde)

icons user on screenshot: [papirus](https://github.com/PapirusDevelopmentTeam/papirus-icon-theme) (recommended)
## usage
### dependencies 
- qt6
- qt6-multimedia
- libtag
### build
Run `build.sh`

#### **or**
```shell
qmake
<<<<<<< HEAD
make
=======
make --jobs=$(grep -c ^processor /proc/cpuinfo)
./potatoMusic
>>>>>>> b08e5ec (updated readme for faster build guide)
```
### run
Run `./potatoMusic` after building
### install
Run `make install` afrer building to install. `make uninstall` to uninstall
## license
potatoMusic © 2024 by slashpotato is licensed under CC BY-NC-SA 4.0 
