#!/bin/bash

git fetch

if git status | grep 'behind' > /dev/null; then
  git pull
fi

./build.sh
