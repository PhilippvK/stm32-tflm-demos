# TFLite Micro Compiler

The TFLM Compiler to build offline models wich can be run without the TFLM Interpreter can be used in the examples.

## Further Information

See official repo by [cpetig](https://github.com/cpetig/tflite_micro_compiler) or fork by [tum-ei-eda](https://github.com/tum-ei-eda/tflite_micro_compiler).

**Info:** Due to some fixes, the submodule in the examples currently used the following extended version of the compiler:

https://github.com/PhilippvK/tflite_micro_compiler

## Usage

Detailed Documantation can be found in the `external/README.md` of each example. F.e. [here](https://github.com/PhilippvK/stm32-tflm-hello-world/blob/94433230c9e1ad586458ae9c107a8a39d30395e0/external/README.md)

Here are the very short summary of the most important steps:

1. Run `git submodule update --init --recursive`
2. Make sure that the dependencies are loaded via running: `download_dependencies.sh`
3. Set `SET(ENABLE_TFLM_COMPILER ON)` after `FIND_PACKAGE(TFLite REQUIRED)` to enable the feature in the `CMakeLists.txt`
4. Convert your TFLite model via `make convert FILE=model.tflite`
5. Try out the generate model under `Src/offline_model.cc`

## Problems

Unfortunately from the start of October 20, due to the changes in []() listed below, the compiler produces Code which intruduces namespace-issues into the compile process.
Until there is a fix available for the compiler, you might have to manually adapt the generated offline model file like explained later.

**Related commits:**
- https://github.com/tensorflow/tensorflow/commit/c95c69473f32e89b8d4b75f822f459afb97d0860
- https://github.com/tensorflow/tensorflow/commit/b13e49836a92241d653e96225552bd8df3bfd92e and similar

**Examples fixes in `offline_model.cc`:**
- Remove replace the prefix `tflite::ops::micro::` with ``tflite::` for the following registration methods which have been moved to a flat namespace:
  - `Register_CONV_2D`
  - `Register_QUANTIZE`
  - `Register_SHAPE`
  - `Register_SOFTMAX`
  - `Register_SVDF`
  - `Register_FULLY_CONNECTED`
- Add `#include "tensorflow/lite/micro/kernels/fully_connected.h"` at the beginning at the file if you need to use the `fully_connected` kernel.
