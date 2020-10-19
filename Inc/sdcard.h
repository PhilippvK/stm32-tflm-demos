/**
  ******************************************************************************
  * @file           : sdcard.h
  * @brief          : Header for sdcard.c file.
  *                   TODO
  ******************************************************************************
  * @attention
  *
  * TODO
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SDCARD_H
#define __SDCRAD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
#include "ff_gen_drv.h"
#include "sd_diskio.h"


/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
uint32_t get_files(const char* path, const char* extension, char* files[], FSIZE_t sizes[]);
uint8_t* get_data(const char* path, char* file, FSIZE_t size);
uint32_t test_txt_file(void);

/* Private defines -----------------------------------------------------------*/
#define MAX_FILES 16
#define MAX_FILENAME_LEN 32
#define MAX_PATH_LEN 64

#ifdef __cplusplus
}
#endif

#endif /* __SDCARD_H */
