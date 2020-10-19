/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#include "tensorflow/lite/micro/kernels/micro_ops.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

#include "main_functions.h"

#include "constants.h"
#include "model_data.h"

// TODO
#include "misc.h"
// Globals, used for compatibility with Arduino-style sketches.
namespace
{
tflite::ErrorReporter *error_reporter = nullptr;
const tflite::Model *model = nullptr;
tflite::MicroInterpreter *interpreter = nullptr;
TfLiteTensor *input = nullptr;
TfLiteTensor *output = nullptr;
int inference_count = 0;

// Create an area of memory to use for input, output, and intermediate arrays.
// Finding the minimum value for your model may require some trial and error.
constexpr int kTensorArenaSize = 16 * 1024;
uint8_t tensor_arena[kTensorArenaSize];
} // namespace

// The name of this function is important for Arduino compatibility.
void setup()
{
	// Set up logging. Google style is to avoid globals or statics because of
	// lifetime uncertainty, but since this has a trivial destructor it's okay.
	// NOLINTNEXTLINE(runtime-global-variables)
	static tflite::MicroErrorReporter micro_error_reporter;
	error_reporter = &micro_error_reporter;

	error_reporter->Report("Hello from the error reporter");

	// Map the model into a usable data structure. This doesn't involve any
	// copying or parsing, it's a very lightweight operation.
	model = tflite::GetModel(model_data_tflite);
	if (model->version() != TFLITE_SCHEMA_VERSION) {
		error_reporter->Report(
			"Model provided is schema version %d not equal "
			"to supported version %d.",
			model->version(), TFLITE_SCHEMA_VERSION);
		return;
	}

    //static tflite::MicroOpResolver<4> resolver;
    static tflite::MicroMutableOpResolver<6> resolver;
    resolver.AddConv2D();
    resolver.AddMaxPool2D();
    resolver.AddReshape();
    resolver.AddFullyConnected();
    resolver.AddSoftmax();
    //resolver.AddQuantize();
    //resolver.AddBuiltin(tflite::BuiltinOperator_DEPTHWISE_CONV_2D,
    //        tflite::ops::micro::Register_DEPTHWISE_CONV_2D());
    //resolver.AddBuiltin(tflite::BuiltinOperator_MAX_POOL_2D,
    //        tflite::ops::micro::Register_MAX_POOL_2D());
    //resolver.AddBuiltin(tflite::BuiltinOperator_FULLY_CONNECTED,
    //        tflite::ops::micro::Register_FULLY_CONNECTED(), 1, 3);
    //resolver.AddBuiltin(tflite::BuiltinOperator_SOFTMAX,
    //        tflite::ops::micro::Register_SOFTMAX());

	// Build an interpreter to run the model with.
	static tflite::MicroInterpreter static_interpreter(model, resolver,
							   tensor_arena,
							   kTensorArenaSize,
							   error_reporter);
	interpreter = &static_interpreter;

	// Allocate memory from the tensor_arena for the model's tensors.
	TfLiteStatus allocate_status = interpreter->AllocateTensors();
	if (allocate_status != kTfLiteOk) {
		error_reporter->Report("AllocateTensors() failed");
		return;
	}

	// Obtain pointers to the model's input and output tensors.
	input = interpreter->input(0);
	output = interpreter->output(0);

	// Keep track of how many inferences we have performed.
	inference_count = 0;

  // Prepare Screen
  DrawInputBox();
#ifndef FAKE_TOUCH
  PrintMNISTHint();
#endif /* FAKE_TOUCH */
}

// The name of this function is important for Arduino compatibility.
void loop()
{
	static TfLiteStatus invoke_status;

  MNISTReset();
#ifndef FAKE_TOUCH
  while(!GetTouchInput());
#endif /* FAKE_TOUCH */
  fprintf(stderr, "Save image\n\r");
  SaveMNISTInput();
  // Place our calculated x value in the model's input tensor
	for (uint32_t i = 0; i < INPUT_IMAGE_SIZE * INPUT_IMAGE_SIZE; i++)
		input->data.int8[i] = (int8_t)(MNISTGetNNInputImage()[i]/2)-127;

	// Run inference, and report any error
	invoke_status = interpreter->Invoke();
	if (invoke_status != kTfLiteOk) {
		error_reporter->Report("Invoke failed");
	}

  int8_t* output_array = output->data.int8;

  MNISTHandleOutput(output_array);
}
