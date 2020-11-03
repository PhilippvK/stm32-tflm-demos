# TFLite Micro Compiler

The TFLM Compiler to build offline models wich can be run without the TFLM Interpreter can be used in the examples.

## Further Information

See official repo by [cpetig](https://github.com/cpetig/tflite_micro_compiler) or fork by [tum-ei-eda](https://github.com/tum-ei-eda/tflite_micro_compiler).

**Info:** Due to some fixes, the submodule in the examples currently used the following extended version of the compiler:

https://github.com/PhilippvK/tflite_micro_compiler

## Usage

TODO

## Problems

Unfortunately from the start of October 20, due to the changes in []() listed below, the compiler produces Code which intruduces namespace-issues into the compile process.
Until there is a fix available for the compiler, you might have to manually adapt the generated offline model file like explained later.

**Related commits:**
- https://github.com/tensorflow/tensorflow/commit/c95c69473f32e89b8d4b75f822f459afb97d0860
- https://github.com/tensorflow/tensorflow/commit/b13e49836a92241d653e96225552bd8df3bfd92e and similar

For `TfLiteRegistration Register_CONV_2D();
TfLiteRegistration Register_DEPTHWISE_CONV_2D();
TfLiteRegistration Register_QUANTIZE();
TfLiteRegistration Register_SHAPE();
TfLiteRegistration Register_SOFTMAX();
TfLiteRegistration Register_SVDF();`

**Examples fixes in `offline_model.cc`:**
- Remove replace the prefix `tflite::ops::micro::` with ``tflite::` for the following registration methods which have been moved to a flat namespace:
  - `Register_CONV_2D`
  - `Register_QUANTIZE`
  - `Register_SHAPE`
  - `Register_SOFTMAX`
  - `Register_SVDF`
  - `Register_FULLY_CONNECTED`
- Add `#include "tensorflow/lite/micro/kernels/fully_connected.h"` at the beginning at the file if you need to use the `fully_connected` kernel.
