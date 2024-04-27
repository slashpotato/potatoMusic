#!/bin/bash

# Параметры
REPO="slashpotato/potatoMusic"
URL=$(curl -s "https://api.github.com/repos/$REPO/releases/latest" | jq -r '.assets[0].browser_download_url')
PKGBUILD="PKGBUILD"

# Загрузка архива
wget "$URL" -O temp.tar.gz

# Вычисление SHA256 суммы
SHA256=$(sha256sum temp.tar.gz | awk '{print $1}')

# Вставка SHA256 суммы в PKGBUILD
sed -i "s/sha256sums=('[^']*')/sha256sums=('$SHA256')/" "$PKGBUILD"
git add PKGBUILD

# Удаление временного архива
rm temp.tar.gz

echo "SHA256 generated to $PKGBUILD"

