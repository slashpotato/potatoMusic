#!/bin/bash

git fetch &> /dev/null

if git status | grep 'behind' > /dev/null; then
  echo "Update found."
  git pull &> /dev/null && echo "Successfully updated."
  if [ -f $PWD/potatoMusic ]; then
    echo "Binary found and will be re-built."
    echo ""
    ./build.sh
  fi
else
  echo "No updates found."
fi
