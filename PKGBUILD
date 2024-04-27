# Maintainer: slashpotato <slashpotato@duck.com>

pkgname=potatoMusic
pkgver=0.4
pkgrel=1
pkgdesc="An qt music player for linux"
arch=('x86_64')
url="https://github.com/slashpotato/potatoMusic"
license=('CC-BY-NC-SA-4.0')
depends=('qt6-base' 'ffmpeg' 'taglib' 'qt6-multimedia')

source=("$pkgname-$pkgver.tar.gz::$url/archive/$pkgver.tar.gz")
sha256sums=('ca927dd237550c56fff6e6bc8e2ef93d5b47bac80160c5e7b8534ed663ba7ef2')

build() {
  cd "$pkgname-$pkgver"
  qmake6
  make
}

package() {
  cd "$pkgname-$pkgver"
  make INSTALL_ROOT="$pkgdir/" install
}

# vim: ts=2 sw=2 et:
