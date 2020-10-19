# Known-Issues & TODO's

## FreeRTOS
The current implementation is bare-metal. FreeRTOS would enable higher-level functionality, such as a USB-accesible file system.

## Issues
- [ ] UART only works with `fprintf`to `stderr`, and not with `printf`

## Fixes
    1. Pull specific TF commit: `cmake -DTF_COMMIT (hash)` 
    2. Delete _deps folder