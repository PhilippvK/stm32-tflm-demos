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

#include "audio_provider.h"

#include "stm32f769i_discovery_audio.h"
#include "stm32f769i_discovery_sdram.h"

#include "micro_features/micro_model_settings.h"
#include <string.h>

#include "main.h"

namespace {

bool g_is_audio_initialized = false;
constexpr int kAudioCaptureBufferSize = kAudioSampleFrequency * 2;
int16_t g_audio_capture_buffer[kAudioCaptureBufferSize];
int16_t g_audio_output_buffer[kMaxAudioSampleSize];
int32_t g_latest_audio_timestamp = 0;

// For a full example of how to access audio on the STM32F746NG board, see
// https://os.mbed.com/teams/ST/code/DISCO-F746NG_AUDIO_demo/
//AUDIO_DISCO_F746NG g_audio_device;
//SDRAM_DISCO_F746NG g_sdram_device;

typedef enum {
  BUFFER_OFFSET_NONE = 0,
  BUFFER_OFFSET_HALF = 1,
  BUFFER_OFFSET_FULL = 2,
} BUFFER_StateTypeDef;

#define AUDIO_BLOCK_SIZE ((uint32_t)4096)

#define SDRAM_WRITE_READ_ADDR        ((uint32_t)0xC0177000)
#define AUDIO_REC_START_ADDR SDRAM_WRITE_READ_ADDR
#define AUDIO_BUFFER_IN AUDIO_REC_START_ADDR /* In SDRAM */
#define AUDIO_BUFFER_OUT \
  (AUDIO_REC_START_ADDR + (AUDIO_BLOCK_SIZE * 2)) /* In SDRAM */
__IO uint32_t g_audio_rec_buffer_state = BUFFER_OFFSET_NONE;

#define SCRATCH_BUFF_SIZE  1024

#if defined ( __CC_ARM )  /* !< ARM Compiler */
int32_t Scratch [SCRATCH_BUFF_SIZE] __attribute__((at(0x2000E000)));
#elif defined ( __ICCARM__ )  /* !< ICCARM Compiler */
#pragma location=0x2000E000
int32_t Scratch [SCRATCH_BUFF_SIZE];
#elif defined ( __GNUC__ )  /* !< GNU Compiler */
int32_t Scratch [SCRATCH_BUFF_SIZE] __attribute__((section(".scratch_section")));
#endif

TfLiteStatus InitAudioRecording(tflite::ErrorReporter* error_reporter) {
#ifndef FAKE_MIC
  BSP_LED_Init((Led_TypeDef)0);
  BSP_LED_Init((Led_TypeDef)1);
  BSP_LED_On((Led_TypeDef)0);
  BSP_LED_On((Led_TypeDef)1);

  /* SDRAM device configuration */
  if(BSP_SDRAM_Init() != SDRAM_OK)
  {
    while(1);
  }
  memset((uint16_t*)AUDIO_BUFFER_IN, 0, AUDIO_BLOCK_SIZE * 2);
  memset((uint16_t*)AUDIO_BUFFER_OUT, 0, AUDIO_BLOCK_SIZE * 2);

  /* Initialize Audio Recorder */
  if (BSP_AUDIO_IN_Init(SAI_AUDIO_FREQUENCY_16K, DEFAULT_AUDIO_IN_BIT_RESOLUTION, 2) != AUDIO_OK)
  {
    while(1);
  }
  BSP_AUDIO_IN_AllocScratch (Scratch, SCRATCH_BUFF_SIZE);
  //BSP_AUDIO_IN_SetVolume(25);

  g_audio_rec_buffer_state = BUFFER_OFFSET_NONE;

  // Start Recording.
  BSP_AUDIO_IN_Record((uint16_t*)AUDIO_BUFFER_IN, AUDIO_BLOCK_SIZE);

  // Also play results out to headphone jack.
  BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE, 50, SAI_AUDIO_FREQUENCY_16K);
  BSP_AUDIO_OUT_SetAudioFrameSlot(CODEC_AUDIOFRAME_SLOT_02);
  BSP_AUDIO_OUT_Play((uint16_t*)AUDIO_BUFFER_OUT, AUDIO_BLOCK_SIZE * 2);
#endif

  return kTfLiteOk;
}

// TODO
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
    audio_magnitude = y_center + (int)(samples[16000*(kFeatureSliceDurationMs-kFeatureSliceStrideMs)/1000+i*stride]>>9);
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

void CaptureSamples(const int16_t* sample_data) {
  const int sample_size = AUDIO_BLOCK_SIZE / (sizeof(int16_t) * 2);
  const int32_t time_in_ms =
      g_latest_audio_timestamp + (sample_size / (kAudioSampleFrequency / 1000));

  const int32_t start_sample_offset =
      g_latest_audio_timestamp * (kAudioSampleFrequency / 1000);
  BSP_LED_Toggle((Led_TypeDef)0);
  for (int i = 0; i < sample_size; ++i) {
    const int capture_index =
        (start_sample_offset + i) % kAudioCaptureBufferSize;
    g_audio_capture_buffer[capture_index] =
        (sample_data[(i * 2) + 0] / 2) + (sample_data[(i * 2) + 1] / 2);
  }
  //plot_wave(sample_data_mono, sample_size);
  // This is how we let the outside world know that new audio data has arrived.
  g_latest_audio_timestamp = time_in_ms;
}

}  // namespace

// These callbacks need to be linkable symbols, because they override weak
// default versions.
void BSP_AUDIO_IN_TransferComplete_CallBack(void) {
  g_audio_rec_buffer_state = BUFFER_OFFSET_FULL;
  /* Copy recorded 1st half block */
  memcpy((uint16_t*)(AUDIO_BUFFER_OUT), (uint16_t*)(AUDIO_BUFFER_IN),
         AUDIO_BLOCK_SIZE);
  CaptureSamples(reinterpret_cast<int16_t*>(AUDIO_BUFFER_IN));
  return;
}

// Another weak symbol override.
void BSP_AUDIO_IN_HalfTransfer_CallBack(void) {
  g_audio_rec_buffer_state = BUFFER_OFFSET_HALF;
  /* Copy recorded 2nd half block */
  memcpy((uint16_t*)(AUDIO_BUFFER_OUT + (AUDIO_BLOCK_SIZE)),
         (uint16_t*)(AUDIO_BUFFER_IN + (AUDIO_BLOCK_SIZE)), AUDIO_BLOCK_SIZE);
  CaptureSamples(
      reinterpret_cast<int16_t*>(AUDIO_BUFFER_IN + AUDIO_BLOCK_SIZE));
  return;
}

// TODO
void AudioSamples(uint8_t* buf) {
  CaptureSamples(reinterpret_cast<int16_t*>(buf));
}


/**
  * @brief  Audio IN Error callback function.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_Error_Callback(void)
{
  /* This function is called when an Interrupt due to transfer error on or peripheral
     error occurs. */
  /* Display message on the LCD screen */
  //BSP_LCD_SetBackColor(LCD_COLOR_RED);
  //BSP_LCD_DisplayStringAt(0, LINE(14), (uint8_t *)"       DMA  ERROR     ", CENTER_MODE);
  while(1);
  /* Stop the program with an infinite loop */
  //while (BSP_PB_GetState(BUTTON_WAKEUP) != RESET)
  //{
  //  return;
  //}
  /* could also generate a system reset to recover from the error */
  /* .... */
}

// Main entry point for getting audio data.
TfLiteStatus GetAudioSamples(tflite::ErrorReporter* error_reporter,
                             int start_ms, int duration_ms,
                             int* audio_samples_size, int16_t** audio_samples) {
  if (!g_is_audio_initialized) {
    TfLiteStatus init_status = InitAudioRecording(error_reporter);
    if (init_status != kTfLiteOk) {
      return init_status;
    }
    g_is_audio_initialized = true;
  }
  // This should only be called when the main thread notices that the latest
  // audio sample data timestamp has changed, so that there's new data in the
  // capture ring buffer. The ring buffer will eventually wrap around and
  // overwrite the data, but the assumption is that the main thread is checking
  // often enough and the buffer is large enough that this call will be made
  // before that happens.
  const int start_offset = start_ms * (kAudioSampleFrequency / 1000);
  const int duration_sample_count =
      duration_ms * (kAudioSampleFrequency / 1000);
  BSP_LED_Toggle((Led_TypeDef)1);
  //TF_LITE_REPORT_ERROR(error_reporter, "OUTPUT");
  for (int i = 0; i < duration_sample_count; ++i) {
    const int capture_index = (start_offset + i) % kAudioCaptureBufferSize;
    g_audio_output_buffer[i] = g_audio_capture_buffer[capture_index];
  }

  *audio_samples_size = kMaxAudioSampleSize;
  *audio_samples = g_audio_output_buffer;
  return kTfLiteOk;
}

int32_t LatestAudioTimestamp() { return g_latest_audio_timestamp; }
