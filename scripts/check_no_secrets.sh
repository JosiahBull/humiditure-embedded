#!/usr/bin/env bash

# Check if there are any secrets in the project
# If there are, exit with error

# check if __SSID__HERE is in the file main.ino
if ! grep -q "__SSID__HERE" ./src/main.ino; then
  echo "The ssid secret is set in the project"
  exit 1
fi

if ! grep -q "__PASSWORD_HERE" ./src/main.ino; then
  echo "The password secret is set in the project"
  exit 1
fi

if ! grep -q "__SERVER_ADDRESS_HERE" ./src/main.ino; then
  echo "The address secret is set in the project"
  exit 1
fi

exit 0
