# STM32 TensorFlow Lite Micro Demos
Collection of STM32 projects making use of Tensorflow Lite Micro. Examples inspired by the [TinyML Book](https://www.oreilly.com/library/view/tinyml/9781492052036/).

## Toolchain:
CMake-based. ~See our template: https://github.com/alxhoff/STM3240G-EVAL-TensorFlow-Hello-World~

## Supported Examples
- [Hello World](https://github.com/PhilippvK/stm32-tflm-hello-world)

  based on [tensorflow/lite/micro/examples/hello_world](https://github.com/tensorflow/tensorflow/tree/master/tensorflow/lite/micro/examples/hello_world)
- [Micro Speech](https://github.com/PhilippvK/stm32-tflm-micro-speech)

  based on [tensorflow/lite/micro/examples/micro_speech](https://github.com/tensorflow/tensorflow/tree/master/tensorflow/lite/micro/examples/micro_speech)
- [MNIST](https://github.com/PhilippvK/stm32-tflm-mnist)

  based on [alxhoff/STM3240G-EVAL-TensorFlow-MNIST](https://github.com/alxhoff/STM3240G-EVAL-TensorFlow-MNIST)

## Supported Boards
- [STM32F413H-Discovery](https://www.st.com/content/st_com/en/products/microcontrollers-microprocessors/stm32-32-bit-arm-cortex-mcus/stm32-high-performance-mcus/stm32f4-series/stm32f413-423/stm32f413zh.html#documentation) (`disco_f413zh`)
- [STM32F769I-Discovery](https://www.st.com/content/st_com/en/products/microcontrollers-microprocessors/stm32-32-bit-arm-cortex-mcus/stm32-high-performance-mcus/stm32f7-series/stm32f7x9/stm32f769ni.html#documentation) (`disco_f769ni`)

Please consult the manufacturer's website for specs and datasheets.

## Running
```
git clone https://github.com/PhilippvK/stm32-tflm-demos
```
You need to also pull the sub-repos where the actual examples are stored.
```
git submodule update --init --recursive
```
Then switch into the folder where the example you want to deploy is stored. Do not worry about the *recursive* argument, we have only linked our repos, which are essential in this case.
```
cd examples/hello_world
```
OR
```
cd examples/micro_speech
```
OR
```
cd examples/mnist
```
Then:
```
mkdir build
cd build
cmake -DTF_COMMIT=37c2bf5016fcbed261476386eced503e907cdc01 ..
make
make flash
```

Other make targets we provide include `make debug` and `make convert` (to convert the `*.tflite` file into [tfite_micro_compiler](https://github.com/tum-ei-eda/tflite_micro_compiler) sources).

## Compatibility
Due to recent updates of the TF Source tree which have not been implemented for the TFLM Compiler, you have to use Commit `37c2bf5016fcbed261476386eced503e907cdc01` or earlier when cloning the TF repositiory via CMake:

```
cd build
cmake -DTF_COMMIT=37c2bf5016fcbed261476386eced503e907cdc01 ..
```

If there are new changes for patched files in the TF Source Tree, they will probably not be merged with the modified file or patches might get overwritten. If something like this occours, please remove the `build/_deps/tf-*` directories and the `CMakeCache.txt` and try again!

## Documentation
[Usage](docs/Usage.md)  
[Debugging](docs/Debugging.md)
[File Structure](docs/File-Structure.md)  
[Issues & TODO](docs/Known-Issues-TODOs.md)  
