#!/bin/bash

git fetch

if git status | grep 'behind' > /dev/null; then
  echo "Update found."
  echo ""
  git pull
  if [ -f $PWD/potatoMusic ]; then
    echo "Binary found and will be re-built."
    echo ""
    ./build.sh
  fi
else
  echo "No updates found."
fi
