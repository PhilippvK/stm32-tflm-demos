# Evaluation of Performance Metrics

## Warnings

1. The methods used to gain the following numbers to evaluate the models in different scenarios are far away from optimal. They are explained shortly at the end of the document, but should be revised to get more accurate results.
2. The on-board results are recorded with the TFLite Interpreter only!

## Board Metrics

| **Metric / Board** | STM32**F4**13HDISCOVERY | STM32**F7**69IDISCOVERY | Unit  |
|--------------------|----------------------|----------------------------|-------|
| Clock Frequency    | 100 MHz              | 216                        | *MHz* |
| Special Features   | -                    | Double Issue, I/D-Cache    | *-*   |
| Flash Memory       | 1.5                  | 2                          | *MB*  |
| SRAM Memory        | 256                  | 512                        | *kB*  |

## Memory Usage

| **Type / Example**      | `hello_world` | `mirco_speech` | `mnist` | **Unit** |
|-------------------------|---------------|----------------|---------|----------|
| Model Size *(FLASH)*    | 2             | 18             | 23      | *kB*     |
| Runtime Memory *(SRAM)* | 1             | 7              | 11      | *kB*     |

According to [this](https://arxiv.org/pdf/2010.11267.pdf) paper, the approximated Memory requirements for TFLite Micro are the following:

- **SRAM:** ~4 *kB*
- **FLASH:** ~37 *kB*


The FLOPS (not FLOP/s) of the model graph should give an idea on how many additions and multiplications are needed for a single invocation on the network. The stated values are highly approximatively as they do not take every TF operation into account and are generated before quantization on the eras models itself.

| `hello_world` | `mirco_speech` | `mnist`        |
|---------------|----------------|----------------|
| 41 *FLOPS*    | 689980 *FLOPS* | 202810 *FLOPS* |


## Runtime Measurements:

The time spend in different sections on the main program `loop()` is analyzed qunatitatively in the following section. The results are heavily influenced by the implementation of the network, as interrupts, serial communication and I/O-activity may be present during the measurements. Therefore and because of the reasons mentioned above, the relationship between the FLOPS and the invocation time is not linear.

All values have been collected by averaging over ~1 minute of runtime with the CMSIS-NN kernels enabled.

The program loop was splitted in the following intervals for all 3 examples:

- `POPULATE`
- `INVOKE`
- `RESPOND`

| **Section / Example** | `hello_world`    | `mirco_speech`   | `mnist`           | **Unit** |
|-----------------------|------------------|------------------|-------------------|----------|
| `POPULATE`            | F4: ~0<br>F7: ~0 | F4: 38<br>F7: 11 | F4: 132<br>F7: 88 | *ms*     |
| `INVOKE`              | F4: ~0<br>F7: ~0 | F4: 49<br>F7: 52 | F4: 34<br>F7: 13  | *ms*     |
| `RESPOND`             | F4: ~0<br>F7: ~0 | F4: ~0<br>F7: ~0 | F4: 125<br>F7: 93 | *ms*     |

## CMSIS-NN Optimizations:

In the following, the differences in execution time for running the model with the [`cmsis-nn`](https://github.com/tensorflow/tensorflow/tree/master/tensorflow/lite/micro/kernels/cmsis-nn) kernels turned ON/OFF are compared briefly. The extend of the improvement depends on the used operations in the graph because only the following kernels are currently supported:

- `add`
- `conv`
- `depthwise_conv.cc`
- `fully_connected`
- `mul`
- `pooling`
- `softmax`
- `svdf`

For details on ARM's neural network optimizations, see: [ARM-software/CMSIS_5](https://github.com/ARM-software/CMSIS_5/tree/develop/CMSIS/NN)

*Results using the STM32F413HDISCOVERY are omitted because they are relatively similar to the F7 values*

### STM32F769IDISCOVERY

| **Setting / Board** | `hello_world` | `mirco_speech` | `mnist`      | **Unit** |
|---------------------|---------------|----------------|--------------|----------|
| `CMSIS_NN=OFF`      | ~1            | 413 (*)        | 52           | *ms*     |
| `CMSIS_NN=ON`       | ~0            | 52             | 13           | *ms*     |
| Difference          | (?)           | (-87)          | (-75)        | *%*      |

* The model is unusable without the CMSIS-NN optimized kernels as the program loop does not run often enough to keep up with the incoming audio stream.

## How to find/calculate the Numbers?

TODO
