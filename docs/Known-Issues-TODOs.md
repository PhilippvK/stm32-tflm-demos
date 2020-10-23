# Known-Issues & TODO's

## FreeRTOS
The current implementation is bare-metal. FreeRTOS would enable higher-level functionality, such as a USB-accesible file system.

## Issues
- [ ] UART only works with `fprintf`to `stderr`, and not with `printf`

Example-specific Issues can be dound in the respective repository!

## Fixes

If recent changes in the Tensorflow source tree broke the build system, do the following

0. Find out Git-Hash of last known working version of `tensorflow`
1. Delete `build/_deps` folder
1. Pull specific TF commit: `cmake -DTF_COMMIT INSERT_HASH_HERE` 
