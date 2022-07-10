#!/usr/bin/env bash

# This script restores the file after building has completed

# restore backup copy of main.ino
cp src/main.ino.bak src/main.ino
rm src/main.ino.bak