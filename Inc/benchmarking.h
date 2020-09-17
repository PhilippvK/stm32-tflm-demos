/* USER CODE BEGIN Header */
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
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BENCHMARKING_H
#define __BENCHMARKING_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
#define AVG_LEN 4
typedef struct ticks_info {
  int32_t cur = -1;
  int32_t avg = -1;
  int32_t min = 10000;
  int32_t max = -1;
  int32_t ticks[AVG_LEN] = {0};
  int32_t index = 0;
  int32_t sum = 0;
  int32_t count = 0;
} ticks_info_t;


/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void print_summary(int types);
void update_avg_ticks(int type, int32_t ticks);
int32_t get_cur_ticks(int type);
int32_t get_avg_ticks(int type);
int32_t get_min_ticks(int type);
int32_t get_max_ticks(int type);

/* Private defines -----------------------------------------------------------*/
#define TYPE_COUNT 4
#define TICKS_POPULATE 1
#define TICKS_INVOKE 2
#define TICKS_RESPOND 4
#define TICKS_OTHER 8

#ifdef __cplusplus
}
#endif

#endif /* __BENCHMARKING_H */
