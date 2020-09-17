// This file is generated. Do not edit.
// Generated on: 16.09.2020 18:33:43

#ifndef micro_speech_GEN_H
#define micro_speech_GEN_H

#include "tensorflow/lite/c/common.h"

// Sets up the model with init and prepare steps.
TfLiteStatus micro_speech_init();
// Returns the input tensor with the given index.
TfLiteTensor *micro_speech_input(int index);
// Returns the output tensor with the given index.
TfLiteTensor *micro_speech_output(int index);
// Runs inference for the model.
TfLiteStatus micro_speech_invoke();

// Returns the number of input tensors.
inline size_t micro_speech_inputs() {
  return 1;
}
// Returns the number of output tensors.
inline size_t micro_speech_outputs() {
  return 1;
}

inline void *micro_speech_input_ptr(int index) {
  return micro_speech_input(index)->data.data;
}
inline size_t micro_speech_input_size(int index) {
  return micro_speech_input(index)->bytes;
}
inline int micro_speech_input_dims_len(int index) {
  return micro_speech_input(index)->dims->data[0];
}
inline int *micro_speech_input_dims(int index) {
  return &micro_speech_input(index)->dims->data[1];
}

inline void *micro_speech_output_ptr(int index) {
  return micro_speech_output(index)->data.data;
}
inline size_t micro_speech_output_size(int index) {
  return micro_speech_output(index)->bytes;
}
inline int micro_speech_output_dims_len(int index) {
  return micro_speech_output(index)->dims->data[0];
}
inline int *micro_speech_output_dims(int index) {
  return &micro_speech_output(index)->dims->data[1];
}

#endif
