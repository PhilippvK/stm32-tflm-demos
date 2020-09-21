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

#include "tensorflow/lite/micro/examples/micro_speech/command_responder.h"

#include "stm32f413h_discovery_lcd.h"

//LCD_DISCO_F746NG lcd;

// Track whether the function has run at least once
bool initialized = false;

// When a command is detected, write it to the display and log it to the
// serial port.
void RespondToCommand(tflite::ErrorReporter *error_reporter,
                      int32_t current_time, const char *found_command,
                      uint8_t score, bool is_new_command) {
  // Do this only once
  if (!initialized) {
    //BSP_LCD_Init();
    // Set the background and foreground colors
    //BSP_LCD_Clear(LCD_COLOR_WHITE);
    initialized = true;
  }
  if (is_new_command) {
  //if (1) {
    TF_LITE_REPORT_ERROR(error_reporter, "Heard %s (%d) @%dms", found_command,
                         score, current_time);
    if (*found_command == 'y') {
      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
      BSP_LCD_FillRect(0, LINE(7), BSP_LCD_GetXSize(), LINE(14)-LINE(7));
      BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
      BSP_LCD_DisplayStringAt(0, LINE(10), (uint8_t *)"Heard yes!", CENTER_MODE);
    } else if (*found_command == 'n') {
      BSP_LCD_SetTextColor(LCD_COLOR_RED);
      BSP_LCD_FillRect(0, LINE(7), BSP_LCD_GetXSize(), LINE(14)-LINE(7));
      BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
      BSP_LCD_DisplayStringAt(0, LINE(10), (uint8_t *)"Heard no :(", CENTER_MODE);
    } else if (*found_command == 'u') {
      BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);
      BSP_LCD_FillRect(0, LINE(7), BSP_LCD_GetXSize(), LINE(14)-LINE(7));
      BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
      BSP_LCD_DisplayStringAt(0, LINE(10), (uint8_t *)"Heard unknown", CENTER_MODE);
    } else {
      BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
      BSP_LCD_FillRect(0, LINE(7), BSP_LCD_GetXSize(), LINE(14)-LINE(7));
      BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
      BSP_LCD_DisplayStringAt(0, LINE(10), (uint8_t *)"Heard silence", CENTER_MODE);
    }
  }
}
