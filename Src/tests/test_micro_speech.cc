/* Copyright 2020 The TensorFlow Authors. All Rights Reserved.

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

#include "tensorflow/lite/micro/examples/micro_speech/main_functions.h"

//#include "tensorflow/lite/micro/examples/micro_speech/audio_provider.h"
#include "tensorflow/lite/micro/examples/micro_speech/command_responder.h"
#include "tensorflow/lite/micro/examples/micro_speech/feature_provider.h"
#include "tensorflow/lite/micro/examples/micro_speech/micro_features/micro_model_settings.h"
#include "tensorflow/lite/micro/examples/micro_speech/micro_features/yes_micro_features_data.h"
#include "tensorflow/lite/micro/examples/micro_speech/micro_features/no_micro_features_data.h"
#include "tensorflow/lite/micro/examples/micro_speech/recognize_commands.h"
#ifdef TFLM_MODE_COMPILER
#include "offline_model.h"
#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/kernels/internal/tensor_ctypes.h"
#else
#include "tensorflow/lite/micro/examples/micro_speech/micro_features/model.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"
#endif /* TFLM_MODE_COMPILER */

#ifdef BENCHMARKING
#include "benchmarking.h"
#endif /* BENCHMARKING */
#include "test_micro_speech.h"

// Globals, used for compatibility with Arduino-style sketches.
namespace {
tflite::ErrorReporter* error_reporter = nullptr;
#ifndef TFLM_MODE_COMPILER
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* model_input = nullptr;
#endif /* TFLM_MODE_COMPILER */
//FeatureProvider* feature_provider = nullptr;
RecognizeCommands* recognizer = nullptr;
//int32_t previous_time = 0;

#ifndef TFLM_MODE_COMPILER
// Create an area of memory to use for input, output, and intermediate arrays.
// The size of this will depend on the model you're using, and may need to be
// determined by experimentation.
constexpr int kTensorArenaSize = 10 * 1024;
uint8_t tensor_arena[kTensorArenaSize];
#endif /* TFLM_MODE_COMPILER */
//int8_t feature_buffer[kFeatureElementCount];
int8_t* model_input_buffer = nullptr;
}  // namespace

// The name of this function is important for Arduino compatibility.
void test_setup() {
  // Set up logging. Google style is to avoid globals or statics because of
  // lifetime uncertainty, but since this has a trivial destructor it's okay.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

#ifdef TFLM_MODE_COMPILER
  micro_speech_init();
  model_input_buffer = tflite::GetTensorData<int8_t>(micro_speech_input(0));
#else
  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  model = tflite::GetModel(g_model);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.",
                         model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  // Pull in only the operation implementations we need.
  // This relies on a complete list of all the ops needed by this graph.
  // An easier approach is to just use the AllOpsResolver, but this will
  // incur some penalty in code space for op implementations that are not
  // needed by this graph.
  //
  // tflite::AllOpsResolver resolver;
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::MicroMutableOpResolver<4> micro_op_resolver(error_reporter);
  if (micro_op_resolver.AddDepthwiseConv2D() != kTfLiteOk) {
    return;
  }
  if (micro_op_resolver.AddFullyConnected() != kTfLiteOk) {
    return;
  }
  if (micro_op_resolver.AddSoftmax() != kTfLiteOk) {
    return;
  }
  if (micro_op_resolver.AddReshape() != kTfLiteOk) {
    return;
  }

  // Build an interpreter to run the model with.
  static tflite::MicroInterpreter static_interpreter(
      model, micro_op_resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
    return;
  }

  // Get information about the memory area to use for the model's input.
  model_input = interpreter->input(0);
  if ((model_input->dims->size != 2) || (model_input->dims->data[0] != 1) ||
      (model_input->dims->data[1] !=
       (kFeatureSliceCount * kFeatureSliceSize)) ||
      (model_input->type != kTfLiteInt8)) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Bad input tensor parameters in model");
    return;
  }
  model_input_buffer = model_input->data.int8;
  #endif /* TFLM_MODE_COMPILER */

  // Prepare to access the audio spectrograms from a microphone or other source
  // that will provide the inputs to the neural network.
  // NOLINTNEXTLINE(runtime-global-variables)
  //static FeatureProvider static_feature_provider(kFeatureElementCount,
  //                                               feature_buffer);
  //feature_provider = &static_feature_provider;

  static RecognizeCommands static_recognizer(error_reporter);
  recognizer = &static_recognizer;

  //previous_time = 0;
}

// The name of this function is important for Arduino compatibility.
void test_loop() {
  // Fetch the spectrogram for the current time.
  //const int32_t current_time = LatestAudioTimestamp();
  //int how_many_new_slices = 0;
  static int input_index = 0;
#ifdef BENCHMARKING
  uint32_t ticks_before, ticks_after;
  ticks_before = HAL_GetTick();
#endif /* BENCHMARKING */
  //TfLiteStatus feature_status = feature_provider->PopulateFeatureData(
  //    error_reporter, previous_time, current_time, &how_many_new_slices);
  //if (feature_status != kTfLiteOk) {
  //  TF_LITE_REPORT_ERROR(error_reporter, "Feature generation failed");
  //  return;
  //}
  //int8_t* feature_buffer;

#ifdef BENCHMARKING
  ticks_after = HAL_GetTick();
  update_avg_ticks(TICKS_POPULATE, (int32_t)(ticks_after-ticks_before));
#endif /* BENCHMARKING */
  //previous_time = current_time;
  // If no new audio samples have been received since last time, don't bother
  // running the network model.
  //if (how_many_new_slices == 0) {
  //  return;
  //}

  // Copy feature buffer to input tensor
  for (unsigned int i = 0; i < model_input->bytes; i++) {
    if (input_index == 0) {
      model_input_buffer[i] = g_yes_micro_f2e59fea_nohash_1_data[i];
    } else if (input_index == 1) {
      model_input_buffer[i] = g_no_micro_f9643d42_nohash_4_data[i];
    } else {
      // ERROR
    }
  }
  input_index = (input_index + 1) % 2;

  // Run the model on the spectrogram input and make sure it succeeds.
#ifdef BENCHMARKING
  ticks_before = HAL_GetTick();
#endif /* BENCHMARKING */
#ifdef TFLM_MODE_COMPILER
  micro_speech_invoke();
#else
  TfLiteStatus invoke_status = interpreter->Invoke();
  if (invoke_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed");
    return;
  }
#endif /* TFLM_MODE_COMPILER */
#ifdef BENCHMARKING
  ticks_after = HAL_GetTick();
  update_avg_ticks(TICKS_INVOKE, (int32_t)(ticks_after-ticks_before));
#endif /* BENCHMARKING */
  // Obtain a pointer to the output tensor
#ifdef TFLM_MODE_COMPILER
  TfLiteTensor* output = micro_speech_output(0);
#else
  TfLiteTensor* output = interpreter->output(0);
#endif
  //TF_LITE_MICRO_EXPECT_EQ(2, output->dims->size);
  //TF_LITE_MICRO_EXPECT_EQ(1, output->dims->data[0]);
  //TF_LITE_MICRO_EXPECT_EQ(4, output->dims->data[1]);
  //TF_LITE_MICRO_EXPECT_EQ(kTfLiteInt8, output->type);

  // There are four possible classes in the output, each with a score.
  const int kSilenceIndex = 0;
  const int kUnknownIndex = 1;
  const int kYesIndex = 2;
  const int kNoIndex = 3;

  // Find the current highest scoring category.
  int current_top_index = 0;
  int32_t current_top_score = 0;
  for (int i = 0; i < kCategoryCount; ++i) {
    if (output->data.int8[i] + 128 > current_top_score) {
      current_top_score = output->data.int8[i] + 128;
      current_top_index = i;
    }
  }
  //TF_LITE_MICRO_EXPECT_GT(yes_score, silence_score);
  //TF_LITE_MICRO_EXPECT_GT(yes_score, unknown_score);
  //TF_LITE_MICRO_EXPECT_GT(yes_score, no_score);

  // Determine whether a command was recognized based on the output of inference
#ifdef BENCHMARKING
  ticks_before = HAL_GetTick();
#endif /* BENCHMARKING */
  //TfLiteStatus process_status = recognizer->ProcessLatestResults(
  //    output, current_time, &found_command, &score, &is_new_command);
  //if (process_status != kTfLiteOk) {
  //  TF_LITE_REPORT_ERROR(error_reporter,
  //                       "RecognizeCommands::ProcessLatestResults() failed");
  //  return;
  //}
  // Do something based on the recognized command. The default implementation
  // just prints to the error console, but you should replace this with your
  // own function for a real application.
  RespondToCommand(error_reporter, 0, kCategoryLabels[current_top_index], current_top_score,
                   true);

  // Make sure that the expected "Yes" score is higher than the other classes.
  uint8_t silence_score = output->data.uint8[kSilenceIndex] + 128;
  uint8_t unknown_score = output->data.uint8[kUnknownIndex] + 128;
  uint8_t yes_score = output->data.int8[kYesIndex] + 128;
  uint8_t no_score = output->data.int8[kNoIndex] + 128;
  TF_LITE_REPORT_ERROR(error_reporter,
                         "silence_score=%d unknown_score=%d yes_score=%d no_score=%d",
                         silence_score, unknown_score, yes_score, no_score);
#ifdef BENCHMARKING
  ticks_after = HAL_GetTick();
  update_avg_ticks(TICKS_RESPOND, (int32_t)(ticks_after-ticks_before));
#endif /* BENCHMARKING */
}
