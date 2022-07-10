# Humiditure Embedded

This repository contains code to run on an esp32-cam microcontroller powerful the humiditure sensor suite. It utilises the camera as a ghetto motion sensor, and POSTs data to a configured [webserver](#sister-repositories).

## How to Use
- Connect LED to GPIO 16 (on == motion detected)
- Modify `GENERAL CONFIG` section of `src/main.ino`, with details relevant for your use case.
    - Note that if you're having issues, setting `#define DEBUG true` will help.
- Upload code to esp32.

## Problems

The device may make excess wifi connection/disconnection attempts due to the current motion sensing implementation. This can be improved by altering the implementation of the motion detection, if it becomes an issue in the future.

Also, a lack of CI/CD is quite problematic, it would be good to at least validate the the script can \*build*. This needs to be sorted out at some point.

## Sister Repositories
- [Humiditure-Backend](https://github.com/JosiahBull/humiditure-backend) - Prototype backend webserver to receive information from sensors.
- [Humiditure-Embedded](https://github.com/JosiahBull/humiditure-embedded) - Code for the esp-32 microcontroller inside of a sensor.
- [Humiditure-CAD](https://github.com/sam-taiff/humiditure-cad) - CAD models of circuit boards and 3D components.

## Licensing and Contribution
Unless otherwise specified, all contributions will be licensed under MIT.
