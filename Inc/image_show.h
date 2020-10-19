/**
  ******************************************************************************
  * @file           : audio.h
  * @brief          : Header for audio.c file.
  *                   This file contains the exported function prototypes for audio
  ******************************************************************************
  * @attention
  *
  * TODO
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IMAGE_SHOW_H
#define __IMAGE_SHOW_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void ImageShow(uint8_t* data, uint16_t x, uint16_t y);

#ifdef __cplusplus
}
#endif

#endif /* __IMAGE_SHOW_H */
