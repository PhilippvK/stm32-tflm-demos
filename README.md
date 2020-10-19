# STM32 TensorFlow Lite Micro Hello World + Micro Speech Demos
Collection of STM32 projects making use of Tensorflow Lite Micro. Examples inspired by TinyML Book: https://www.oreilly.com/library/view/tinyml/9781492052036/

**Toolchain:**
CMake-based. See Template: https://github.com/alxhoff/STM3240G-EVAL-TensorFlow-Hello-World

**Supported Examples**
- [Hello World](https://github.com/tensorflow/tensorflow/tree/master/tensorflow/lite/micro/examples/hello_world)
- [Micro Speech](https://github.com/tensorflow/tensorflow/tree/master/tensorflow/lite/micro/examples/micro_speech)
- MNIST

**Supported Boards:**
- STM32F413H-Discovery (`disco_f413zh`)
- STM32F769I-Discovery (`disco_f769ni`)

**Compatibility**

Due to recent updates of the TF Source tree which have not been implemented for the TFLM Compiler, you have to use Commit `37c2bf5016fcbed261476386eced503e907cdc01` or earlier when cloning the TF repositiory via CMake:

```
cd build
cmake -DTF_COMMIT=37c2bf5016fcbed261476386eced503e907cdc01 ..
```

If there are new changes for patched files in the TF Source Tree, they will probably not be merged with the modified file or patches might get overwritten. If something like this occours, please remove the `build/_deps/tf-*` directories and the `CMakeCache.txt` and try again!
