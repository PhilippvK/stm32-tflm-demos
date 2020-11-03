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

| **Type / Example**                    | `hello_world` | `mirco_speech` | `mnist` | **Unit** |
|---------------------------------------|---------------|----------------|---------|----------|
| Model Size *(FLASH)*                  | 2             | 18             | 23      | *kB*     |
| TensorArena Size [HEAD+TAIL] *(SRAM)* | 1             | 7              | 11      | *kB*     |

According to [this](https://arxiv.org/pdf/2010.11267.pdf) paper, the approximated Memory requirements for TFLite Micro are the following:

- **SRAM:** ~4 *kB*
- **FLASH:** ~37 *kB*

## Number of Operations

The FLOPS (not FLOP/s) of the model graph should give an idea on how many additions and multiplications are needed for a single invocation on the network. The stated values are highly approximatively as they do not take every TF operation into account and are generated before quantization on the eras models itself.

| `hello_world` | `mirco_speech` | `mnist`        |
|---------------|----------------|----------------|
| 41 *FLOPS*    | 689980 *FLOPS* | 202810 *FLOPS* |


## Runtime Measurements

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

## CMSIS-NN Optimizations

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

### When using the TFLite Micro Compiler

When building an offline model using f.e. `make convert FILE=model.tflite` the compiler prints some relevant details on the memory usage. Look for `ROM Summary` and `RAM Summary`!

### Model Size

Just use the size of the `uint8_t` array in `model_data.cc`.

### Tensor Arena Size

Set the folloing in your `CMakeLists.txt`:

```
SET(ENABLE_MEMORY_REPORTING ON)
```

The results will be printed via the serial connection, f.e. like this:

```
[RecordingMicroAllocator] Arena allocation total 904 bytes
[RecordingMicroAllocator] Arena allocation head 52 bytes
[RecordingMicroAllocator] Arena allocation tail 852 bytes
[RecordingMicroAllocator] 'TfLiteEvalTensor data' used 144 bytes with alignment overhead (requested 144 bytes for 12 allocations)
[RecordingMicroAllocator] 'Persistent TfLiteTensor data' used 128 bytes with alignment overhead (requested 128 bytes for 2 tensors)
[RecordingMicroAllocator] 'Persistent buffer data' used 212 bytes with alignment overhead (requested 172 bytes for 5 allocations)
[RecordingMicroAllocator] 'NodeAndRegistration struct' used 200 bytes with alignment overhead (requested 200 bytes for 5 NodeAndRegistration structs)
[RecordingMicroAllocator] 'Operator runtime data' used 12 bytes with alignment overhead (requested 12 bytes for 3 OpData structs)
```
For Details, see: [`tensorflow/lite/micro/docs/memory_management.md`](https://github.com/tensorflow/tensorflow/blob/master/tensorflow/lite/micro/docs/memory_management.md)

**Tipp:** Further information can be collected via `arm-none-eabi-size`.

### FLOPS
#### TF1 *(Required for `micro_speech` training)*

Make use of TF Profiler, f.e. like this:

1. Export frozen model graph with type `graph_def` as `saved_model.pb`
2. Import GraphDef File and run `tf.profiler.profile(...)`
```
from tensorflow.python.framework import graph_util

def load_pb(pb):
    with tf.gfile.GFile(pb, "rb") as f:
        graph_def = tf.GraphDef()
        graph_def.ParseFromString(f.read())
    with tf.Graph().as_default() as graph:
        tf.import_graph_def(graph_def, name='')
        return graph
        
# ***** Load graph *****
g = load_pb('./graph.pb')
with g.as_default():
    flops = tf.profiler.profile(g, options = tf.profiler.ProfileOptionBuilder.float_operation())
    print('FLOPS:', flops.total_float_ops)
```

#### TF2

Use modified version of [evgps/flopco-keras](https://github.com/evgps/flopco-keras) found here: https://github.com/alpereninci/OctaveNet/blob/master/OctaveNet.ipynb

1. Define Model: `current_model`
2. Run the following after freezing:

```
if True:
    import numpy as np
    from collections import defaultdict
    from functools import partial
    import copy


    def numel(w : list):
        out = 1
        for k in w:
            out *= k
        return out

    def compute_input_flops(layer, macs = False):
        return 0
    def compute_flatten_flops(layer, macs = False):
        return 0
    def compute_dropout_flops(layer, macs = False):
        return 0
    def compute_padding_flops(layer, macs = False):
        return 0
    def compute_activation_flops(layer, macs = False):
        return 0
    def compute_tfop_flops(layer, macs = False):
        return 0
    def compute_add_flops(layer, macs = False):
        return 0

    def compute_conv2d_flops(layer, macs = False):
        
    #     _, cin, h, w = input_shape
        if layer.data_format == "channels_first":
            _, input_channels, _, _ = layer.input_shape
            _, output_channels, h, w, = layer.output_shape
        elif layer.data_format == "channels_last":
            _, _, _, input_channels = layer.input_shape
            _, h, w, output_channels = layer.output_shape
        
        w_h, w_w =  layer.kernel_size

    #     flops = h * w * output_channels * input_channels * w_h * w_w / (stride**2)
        flops = h * w * output_channels * input_channels * w_h * w_w

        
        if not macs:
            flops_bias = numel(layer.output_shape[1:]) if layer.use_bias is not None else 0
            flops = 2 * flops + flops_bias
            
        return int(flops)
        

    def compute_fc_flops(layer, macs = False):
        ft_in, ft_out =  layer.input_shape[-1], layer.output_shape[-1]
        flops = ft_in * ft_out
        
        if not macs:
            flops_bias = ft_out if layer.use_bias is not None else 0
            flops = 2 * flops + flops_bias
            
        return int(flops)

    def compute_bn2d_flops(layer, macs = False):
        # subtract, divide, gamma, beta
        flops = 2 * numel(layer.input_shape[1:])
        
        if not macs:
            flops *= 2
        
        return int(flops)


    def compute_relu_flops(layer, macs = False):
        
        flops = 0
        if not macs:
            flops = numel(layer.input_shape[1:])

        return int(flops)


    def compute_maxpool2d_flops(layer, macs = False):

        flops = 0
        if not macs:
            flops = layer.pool_size[0]**2 * numel(layer.output_shape[1:])

        return flops


    def compute_pool2d_flops(layer, macs = False):

        flops = 0
        if not macs:
            flops = layer.pool_size[0]**2 * numel(layer.output_shape[1:])

        return flops

    def compute_globalavgpool2d_flops(layer, macs = False):

        if layer.data_format == "channels_first":
            _, input_channels, h, w = layer.input_shape
            _, output_channels = layer.output_shape
        elif layer.data_format == "channels_last":
            _, h, w, input_channels = layer.input_shape
            _, output_channels = layer.output_shape

        return h*w

    def compute_softmax_flops(layer, macs = False):
        
        nfeatures = numel(layer.input_shape[1:])
        
        total_exp = nfeatures # https://stackoverflow.com/questions/3979942/what-is-the-complexity-real-cost-of-exp-in-cmath-compared-to-a-flop
        total_add = nfeatures - 1
        total_div = nfeatures
        
        flops = total_div + total_exp
        
        if not macs:
            flops += total_add
            
        return flops


    class FlopCoKeras():
        
        def __init__(self, model):
            '''
            instances: list of layer types,
                supported types are [nn.Conv2d, nn.Linear,
                nn.BatchNorm2d, nn.ReLU, nn.MaxPool2d, nn.AvgPool2d, nn.Softmax]
            '''
            self.model = model
            
            self.flops = []
            self.macs = []
            self.get_flops = {
                        'ReLU': compute_relu_flops,
                        'InputLayer': compute_input_flops,
                        'Conv2D': compute_conv2d_flops,
                        'ZeroPadding2D': compute_padding_flops,
                        'Activation': compute_activation_flops,
                        'Dense': compute_fc_flops,
                        'BatchNormalization': compute_bn2d_flops,
                        'TensorFlowOpLayer': compute_tfop_flops,
                        'MaxPooling2D': compute_maxpool2d_flops,
                        'AveragePooling2D': compute_pool2d_flops,
                        'Add': compute_add_flops,
                        'Flatten': compute_flatten_flops,
                        'Dropout': compute_dropout_flops,
                        'UpSampling2D': compute_dropout_flops,
                        'GlobalAveragePooling2D': compute_globalavgpool2d_flops}

            
            self.get_stats( flops = True, macs = True)
            
            self.total_flops = sum(self.flops)
            self.total_macs = sum(self.macs)
            # self.total_params = sum(self.params) #TO DO 
            
            self.relative_flops = [k/self.total_flops for k in self.flops]
            
            self.relative_macs = [k/self.total_macs for k in self.macs]
            
            # self.relative_params = [k/self.total_params for k in self.params] #TO DO 

            del self.model        
            
        def __str__(self):
            print_info = "\n".join([str({k:v}) for k,v in self.__dict__.items()])
            
            return str(self.__class__) + ": \n" + print_info               
        
        # def count_params(self):
        #     self.params = [0]
            # self.params = defaultdict(int)
            
            # for mname, m in self.model.named_modules():
            #     if m.__class__ in self.instances:
                    
            #         self.params[mname] = 0
                    
            #         for p in m.parameters():
            #             self.params[mname] += p.numel()
        
        def _save_flops(self, layer, macs=False):
            flops = self.get_flops[layer.__class__.__name__](layer, macs)
            if macs:
                self.macs.append(flops)
            else:
                self.flops.append(flops)
            

        def get_stats(self, flops = False, macs = False):
            
            # if params:
            #     self.count_params()
        
            if flops:
                self.flops = []
            
            if macs:
                self.macs = []

            if flops:
                for layer in self.model.layers:
                    self._save_flops(layer)
            if macs:
                for layer in self.model.layers:
                    self._save_flops(layer, macs=True)

    # MAC : Multiplication and ACcumulation
    # FLOP : Floating Point Operation
    stats = FlopCoKeras(current_model)
    print(f"Model 1 FLOPs: {stats.total_flops}")
    print(f"Model 1 MACs: {stats.total_macs}")
```

### Runtime

Enable Benchmarking in `CMakeLists.txt`:

```
SET(ENABLE_BENCHMARKING ON)
```

The measures miliseconds for the Populate/Invoke/Respond phases for every iteration are printed on the serial terinal like this:

```
TIMESTAMP: 16039 ms
Ticks for Populate (cur/min/max/avg): 15/11/444/13
Ticks for Invoke (cur/min/max/avg): 49/48/50/49
Ticks for Respond (cur/min/max/avg): 6/0/21/13
```
