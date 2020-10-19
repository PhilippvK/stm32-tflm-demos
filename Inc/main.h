/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  * TODO
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>

/* STM32 Board related Includes */
#ifdef STM32_BOARD_STM32F413H_DISCOVERY
#include "stm32f4xx_hal.h"
#include "stm32f413h_discovery.h"
#include "stm32f413h_discovery_lcd.h"
#include "stm32f413h_discovery_ts.h"
#include "stm32f413h_discovery_audio.h"
#include  "stm32f413h_discovery_sd.h"
#endif

#ifdef STM32_BOARD_STM32F769I_DISCOVERY
#include "stm32f7xx_hal.h"
#include "stm32f769i_discovery.h"
#include "stm32f769i_discovery_lcd.h"
#include "stm32f769i_discovery_ts.h"
#include "stm32f769i_discovery_audio.h"
#include  "stm32f769i_discovery_sd.h"
#endif

/* Touchscreen Includes */
#include "ts_calibration.h"

/* FatFs/SD Includes */
#include "ff_gen_drv.h"
#include "sd_diskio.h"

#ifdef FAKE_MIC
/* AudioPlayer Includes */
#include "audio_playback.h"
#endif /* FAKE_MIC */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
int __io_putchar(int ch);
void Error_Handler(void);
uint8_t CheckForUserInput(void);

/* Private defines -----------------------------------------------------------*/
#ifdef STM32_BOARD_STM32F413H_DISCOVERY
#define B_USER_Pin GPIO_PIN_0
#define B_USER_GPIO_Port GPIOA
#define VCP_TX_Pin GPIO_PIN_14
#define VCP_TX_GPIO_Port GPIOG
#define VCP_RX_Pin GPIO_PIN_9
#define VCP_RX_GPIO_Port GPIOG
#define UartHandle huart6
#endif

#ifdef STM32_BOARD_STM32F769I_DISCOVERY
#define B_USER_Pin GPIO_PIN_0
#define B_USER_GPIO_Port GPIOA
#define VCP_TX_Pin GPIO_PIN_9
#define VCP_TX_GPIO_Port GPIOA
#define VCP_RX_Pin GPIO_PIN_10
#define VCP_RX_GPIO_Port GPIOA
#define UartHandle huart1
#endif

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
