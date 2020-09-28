/**
  ******************************************************************************
  * @file           : sdcard.c
  * @brief          : TODO
  *                   TODO
  ******************************************************************************
  * @attention
  *
  * TODO
  *
  ******************************************************************************
  */

/* Private includes ----------------------------------------------------------*/
#include "sdcard.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static FATFS sdfs;  /* File system object for SD card logical drive */
static DIR dir;
static FIL fil;    /* File object */
static FILINFO fno;
static char sdpath[4]; /* SD card logical drive path */

//static uint8_t buffer[_MAX_SS]; /* a work buffer for the f_mkfs() */

/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/

// TODO
uint8_t* get_wav_data(const char* path, char* file, FSIZE_t size) {
  FRESULT res;
  uint32_t bytesread;
  char buf[64];
  uint8_t* dest;

  /* Link the micro SD disk I/O driver */
  if(FATFS_LinkDriver(&SD_Driver, sdpath) != 0) {
    return 0;
  } else {
    /* Open filesystem */
    if(f_mount(&sdfs, (TCHAR const*)sdpath,0) != FR_OK) {
      FATFS_UnLinkDriver(sdpath);
      return 0;
    } else {
      if (!dest) {
        dest = (uint8_t*)malloc(size * sizeof(uint8_t));
        if (!dest) {
          FATFS_UnLinkDriver(sdpath);
          return 0;
        }
      }
      sprintf(buf, "%s/%s", path, file);
      if(f_open(&fil, buf, FA_READ) != FR_OK) {
        FATFS_UnLinkDriver(sdpath);
        return 0;
      } else {
        /* Read data from the file */
        res = f_read(&fil, dest, size, (UINT*)&bytesread);

        if((bytesread != size) || (res != FR_OK)) {
          /* File Read or EOF Error */
          f_close(&fil);
          FATFS_UnLinkDriver(sdpath);
          return 0;
        } else {
          /* Close the open text file */
          f_close(&fil);
        }
      }
    }
  }
  /* Unlink the RAM disk I/O driver */
  FATFS_UnLinkDriver(sdpath);
  return dest;
}

// TODO
uint32_t get_wav_files(const char* path, char* files[], FSIZE_t sizes[]) {
  FRESULT res;
  uint32_t index = 0;

  /* Link the micro SD disk I/O driver */
  if(FATFS_LinkDriver(&SD_Driver, sdpath) != 0) {
    return 0;
  } else {
    /* Open filesystem */
    if(f_mount(&sdfs, (TCHAR const*)sdpath,0) != FR_OK) {
      FATFS_UnLinkDriver(sdpath);
      return 0;
    } else {
      /* Start to search for wave files */
      res = f_findfirst(&dir, &fno, path, "*.wav");

      /* Repeat while an item is found */
      while (fno.fname[0]) {
        if(res == FR_OK) {
          fprintf(stderr, "File: %s", fno.fname);
          if(index < MAX_FILES) {
            if (fno.fname[0]!='.') {
              sizes[index] = fno.fsize;
              files[index] = (char*)calloc(MAX_FILENAME_LEN, sizeof(char));
              if (!files) {
                f_closedir(&dir);
                FATFS_UnLinkDriver(sdpath);
                return 0;
              } else {
                sprintf (files[index++], "%s", fno.fname);
              }
            }
          }
          /* Search for next item */
          res = f_findnext(&dir, &fno);
        } else {
          index = 0;
          break;
        }
      }
      f_closedir(&dir);
    }
  }
  /* Unlink the RAM disk I/O driver */
  FATFS_UnLinkDriver(sdpath);
  return index;
}

uint32_t test_txt_file() {
  FRESULT res;                                          /* FatFs function common result code */
  uint32_t byteswritten, bytesread;                     /* File write/read counts */
  uint8_t wtext[] = "This is STM32 working with FatFs"; /* File write buffer */
  uint8_t rtext[100];
  /* Link the micro SD disk I/O driver */
  if(FATFS_LinkDriver(&SD_Driver, sdpath) == 0) {
    /* Register the file system object to the FatFs module */
    if(f_mount(&sdfs, (TCHAR const*)sdpath, 0) != FR_OK) {
      /* FatFs Initialization Error */
      FATFS_UnLinkDriver(sdpath);
      return 0;
    } else {
      /* Create a FAT file system (format) on the logical drive */
      /* WARNING: Formatting the uSD card will delete all content on the device */
      //if(f_mkfs((TCHAR const*)sdpath, FM_ANY, 0, buffer, sizeof(buffer)) != FR_OK)
      if(0) { // SKIPPED
        /* FatFs Format Error */
        FATFS_UnLinkDriver(sdpath);
        return 0;
      } else {
        /* Create and Open a new text file object with write access */
        if(f_open(&fil, "STM32.TXT", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK) {
          /* 'STM32.TXT' file Open for write Error */
          FATFS_UnLinkDriver(sdpath);
          return 0;
        } else {
          /* Write data to the text file */
          res = f_write(&fil, wtext, sizeof(wtext), (UINT*)&byteswritten);

          /* Close the open text file */
          if (f_close(&fil) != FR_OK ) {
            FATFS_UnLinkDriver(sdpath);
            return 0;
          }

          if((byteswritten == 0) || (res != FR_OK)) {
            /* 'STM32.TXT' file Write or EOF Error */
            return 0;
          } else {
            /* Open the text file object with read access */
            if(f_open(&fil, "STM32.TXT", FA_READ) != FR_OK)
            {
              /* 'STM32.TXT' file Open for read Error */
              FATFS_UnLinkDriver(sdpath);
              return 0;
            } else {
              /* Read data from the text file */
              res = f_read(&fil, rtext, sizeof(rtext), (UINT*)&bytesread);

              if((bytesread == 0) || (res != FR_OK)) {
                /* 'STM32.TXT' file Read or EOF Error */
                f_close(&fil);
                FATFS_UnLinkDriver(sdpath);
                return 0;
              } else {
                /* Close the open text file */
                f_close(&fil);

                /* Compare read data with the expected data */
                if((bytesread != byteswritten)) {
                  /* Read data is different from the expected data */
                  FATFS_UnLinkDriver(sdpath);
                  return 0;
                } else {
                  /* Success of the demo: no error occurrence */
                }
              }
            }
          }
        }
      }
    }
  }
  /* Unlink the RAM disk I/O driver */
  FATFS_UnLinkDriver(sdpath);
  return bytesread;
}
