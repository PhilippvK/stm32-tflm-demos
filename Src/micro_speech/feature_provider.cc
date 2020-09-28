/* Copyright 2018 The TensorFlow Authors. All Rights Reserved.

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

#include "tensorflow/lite/micro/examples/micro_speech/feature_provider.h"

#include "tensorflow/lite/micro/examples/micro_speech/audio_provider.h"
#include "tensorflow/lite/micro/examples/micro_speech/micro_features/micro_features_generator.h"
#include "tensorflow/lite/micro/examples/micro_speech/micro_features/micro_model_settings.h"

// TODO
#include "stm32f413h_discovery.h"
#include "stm32f413h_discovery_lcd.h"
#include <string.h>
#include <stdlib.h>
void plot_wave(int16_t* samples, int size);
void plot_features(int8_t* features, int slices_needed);

FeatureProvider::FeatureProvider(int feature_size, int8_t* feature_data)
    : feature_size_(feature_size),
      feature_data_(feature_data),
      is_first_run_(true) {
  // Initialize the feature data to default values.
  for (int n = 0; n < feature_size_; ++n) {
    feature_data_[n] = 0;
  }
}

FeatureProvider::~FeatureProvider() {}

TfLiteStatus FeatureProvider::PopulateFeatureData(
    tflite::ErrorReporter* error_reporter, int32_t last_time_in_ms,
    int32_t time_in_ms, int* how_many_new_slices) {
  if (feature_size_ != kFeatureElementCount) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Requested feature_data_ size %d doesn't match %d",
                         feature_size_, kFeatureElementCount);
    return kTfLiteError;
  }

  // Quantize the time into steps as long as each window stride, so we can
  // figure out which audio data we need to fetch.
  const int last_step = (last_time_in_ms / kFeatureSliceStrideMs);
  const int current_step = (time_in_ms / kFeatureSliceStrideMs);

  int slices_needed = current_step - last_step;
  // If this is the first call, make sure we don't use any cached information.
  if (is_first_run_) {
    TfLiteStatus init_status = InitializeMicroFeatures(error_reporter);
    if (init_status != kTfLiteOk) {
      return init_status;
    }
    is_first_run_ = false;
    slices_needed = kFeatureSliceCount;
  }
  if (slices_needed > kFeatureSliceCount) {
    slices_needed = kFeatureSliceCount;
  }
  *how_many_new_slices = slices_needed;

  const int slices_to_keep = kFeatureSliceCount - slices_needed;
  const int slices_to_drop = kFeatureSliceCount - slices_to_keep;
  // If we can avoid recalculating some slices, just move the existing data
  // up in the spectrogram, to perform something like this:
  // last time = 80ms          current time = 120ms
  // +-----------+             +-----------+
  // | data@20ms |         --> | data@60ms |
  // +-----------+       --    +-----------+
  // | data@40ms |     --  --> | data@80ms |
  // +-----------+   --  --    +-----------+
  // | data@60ms | --  --      |  <empty>  |
  // +-----------+   --        +-----------+
  // | data@80ms | --          |  <empty>  |
  // +-----------+             +-----------+
  /*TF_LITE_REPORT_ERROR(error_reporter,
                       "---\ncurrent_step=%d | slices_needed=%d | slices_to_keep=%d | slices_to_drop=%d",
                       current_step, slices_needed, slices_to_keep, slices_to_drop);*/
  if (slices_to_keep > 0) {
    for (int dest_slice = 0; dest_slice < slices_to_keep; ++dest_slice) {
      int8_t* dest_slice_data =
          feature_data_ + (dest_slice * kFeatureSliceSize);
      const int src_slice = dest_slice + slices_to_drop;
      const int8_t* src_slice_data =
          feature_data_ + (src_slice * kFeatureSliceSize);
      for (int i = 0; i < kFeatureSliceSize; ++i) {
        dest_slice_data[i] = src_slice_data[i];
      }
    }
  }
  // Any slices that need to be filled in with feature data have their
  // appropriate audio data pulled, and features calculated for that slice.
  if (slices_needed > 0) {
    for (int new_slice = slices_to_keep; new_slice < kFeatureSliceCount;
         ++new_slice) {
      const int new_step = (current_step - kFeatureSliceCount + 1) + new_slice;
      const int32_t slice_start_ms = (new_step * kFeatureSliceStrideMs);
      int16_t* audio_samples = nullptr;
      int audio_samples_size = 0;
      // TODO(petewarden): Fix bug that leads to non-zero slice_start_ms
      GetAudioSamples(error_reporter, (slice_start_ms > 0 ? slice_start_ms : 0),
                      kFeatureSliceDurationMs, &audio_samples_size,
                      &audio_samples);
      if (audio_samples_size < kMaxAudioSampleSize) {
        TF_LITE_REPORT_ERROR(error_reporter,
                             "Audio data size %d too small, want %d",
                             audio_samples_size, kMaxAudioSampleSize);
        return kTfLiteError;
      }
      int8_t* new_slice_data = feature_data_ + (new_slice * kFeatureSliceSize);
      size_t num_samples_read;
      TfLiteStatus generate_status = GenerateMicroFeatures(
          error_reporter, audio_samples, audio_samples_size, kFeatureSliceSize,
          new_slice_data, &num_samples_read);
      if (generate_status != kTfLiteOk) {
        return generate_status;
      }
      //plot_wave(audio_samples, audio_samples_size);
    }
  }
  //plot_features(feature_data_, slices_needed);
  return kTfLiteOk;
}

void plot_wave(int16_t* samples, int size) {
  static int *audio_plot_buffer;

  if (!audio_plot_buffer) {
    audio_plot_buffer = new int[BSP_LCD_GetXSize()];
  }

  int stride = size/BSP_LCD_GetXSize();
  int y_center = BSP_LCD_GetYSize()/6;
  int audio_magnitude = y_center;
  BSP_LCD_FillRect(0,0,BSP_LCD_GetXSize(),BSP_LCD_GetYSize()/3);
  for(uint32_t i=0;i<BSP_LCD_GetXSize();i++)
  {
    audio_magnitude = y_center + (int)(samples[16000*(kFeatureSliceDurationMs-kFeatureSliceStrideMs)/1000+i*stride]/256);
    if (audio_magnitude < 0)
      audio_magnitude = 0;
    if (audio_magnitude > 2*y_center)
      audio_magnitude = 2*y_center - 1;
    audio_plot_buffer[i] = audio_magnitude;
  }
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  for(uint32_t i=0;i<BSP_LCD_GetXSize()-1;i++)
    BSP_LCD_DrawLine(i,audio_plot_buffer[i],i+1,audio_plot_buffer[i+1]);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
}

uint32_t calculate_rgb(int min, int max, int value) {
  if (value<min) {
    value = min;
  } else if (value > max) {
    value = max;
  }
  uint32_t tmp = (value-min);
  return (tmp)+((tmp)<<8)+((tmp)<<16);
  //uint32_t ret = 0xFF000000;
  uint32_t ret = 0x000000;
  int mid_point = (min + max) / 2;
  int range = (max - min);
  if (value >= mid_point) {
    uint32_t delta = (value - mid_point)*512 / range;
    if (delta > 255) {  delta = 255;  }
    ret = ret | (delta << 16);
    ret = ret | ( (255-delta) << 8 );
  } else {
    int delta = value*512 / range;
    if (delta > 255) {  delta = 255;  }
    ret = ret | (delta << 8);
    ret = ret | (255 - delta);
  }
  return ret;
}

void plot_features(int8_t* features, int slices_needed) {
  //const int slices_to_keep = kFeatureSliceCount - slices_needed;
  static uint8_t *feature_plot_buffer;
  //static int mfcc_update_counter=0;
  const int how_many = 3;
  const int limit = 10;
  static int index = 0;

  if (!feature_plot_buffer) {
    feature_plot_buffer = new uint8_t[kFeatureSliceSize*kFeatureSliceCount*how_many];
  }
  memcpy(feature_plot_buffer, feature_plot_buffer+slices_needed*kFeatureSliceSize, kFeatureSliceSize*(kFeatureSliceCount*how_many-slices_needed));
  memcpy(feature_plot_buffer+kFeatureSliceSize*(kFeatureSliceCount*how_many-slices_needed), features+kFeatureSliceSize*(kFeatureSliceCount-slices_needed), slices_needed*kFeatureSliceSize);

  index++;
  if (index == limit) {
    index = 0;
  int x_step = 1;
  int y_step = 1; //6;

  //uint32_t* pBuffer = mfcc_plot_buffer + kFeatureSliceSize*(how_many*kFeatureSliceCount-2);

  //for (int i=0;i<slices_needed;i++) {
  //  for (int j=0;j<kFeatureSliceSize;j++) {
  //    int value = features[(kFeatureSliceSize*(kFeatureSliceCount-2))+i*kFeatureSliceSize+j];
  //    uint32_t RGB  = calculate_rgb(-128, 127, value*4);
  //    sum += abs(value);
  //    pBuffer[i*kFeatureSliceSize+j] = RGB;
  //  }
  //}
  //int x_start = (BSP_LCD_GetXSize() - (kFeatureSliceCount*how_many))/2;
  int x_start = (BSP_LCD_GetXSize() - (kFeatureSliceCount*how_many))/2;
  x_start = (x_start>0) ? x_start:0;
  //mfcc_update_counter++;
  //if(mfcc_update_counter==how_many) {
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_FillRect(0, 200, BSP_LCD_GetXSize(), BSP_LCD_GetYSize()-200);
    for (int i=0;i<how_many*kFeatureSliceCount;i++) {
      for (int j=0;j<kFeatureSliceSize;j++) {
        for (int x=0;x<x_step;x++) {
          for (int y=0;y<y_step;y++) {
            //BSP_LCD_DrawPixel(x_start+i*x_step+x,200+j*y_step+y, mfcc_plot_buffer[i*kFeatureSliceSize+j]);
            uint32_t color = calculate_rgb(-128, 127, feature_plot_buffer[i*kFeatureSliceSize+j]);
            BSP_LCD_DrawPixel(x_start+i*x_step+x,200+j*y_step+y, color);
          }
        }
      }
    }
  //mfcc_update_counter=0;
  //}
  }
}
