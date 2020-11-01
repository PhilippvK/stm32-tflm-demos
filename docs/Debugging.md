# Debugging

## Printing Messages

Serial Logging via `fprintf(stderr, ...)`

Baud Rate: `115200`

Example (MacOS):

```
screen /dev/tty.usbmodem14103 115200
```

In TFLite-related source files, you can also make use of the `TF_LITE_REPORT_ERROR` macro!

## Problems

The latest release of the OpenOCD-Debugger unfortunately does not yet support the STM32F413H-Discovery board. To get the debugger running, you have to build the program from the [sources](https://github.com/ntfreak/openocd) by yourself. The relevant commit/change is the following:

https://github.com/ntfreak/openocd/commit/7719e9618e753ac41a46a2488dfba549ac578891

