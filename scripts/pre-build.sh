#!/usr/bin/env bash

# This script is run before the build starts.


# It replaces some values inside of main.ino with environemntal variables

# load values from .env into the environment
while read -r LINE; do
if [[ $LINE == *'='* ]] && [[ $LINE != '#'* ]]; then
    ENV_VAR="$(echo $LINE | envsubst)"
    eval "declare $ENV_VAR"
fi
done < .env

# make backup copy of main.ino
cp src/main.ino src/main.ino.bak

# replace the value of the variable "MY_VARIABLE" with the value of the environment variable "MY_ENV_VARIABLE"
sed -i "s/WIFI_SSID_ENV/${WIFI_SSID_ENV}/g" src/main.ino
sed -i "s/WIFI_PASSWORD_ENV/${WIFI_PASSWORD_ENV}/g" src/main.ino
sed -i "s/SERVER_ADDRESS_ENV/${SERVER_ADDRESS_ENV}/g" src/main.ino
