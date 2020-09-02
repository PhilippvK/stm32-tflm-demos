/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"
// TODO
#include "data_types.h"
#include "main_functions.h"
#include "constants.h"
#include "color.h"

// TODO: kws
#define KWS_TYPE DS_CNN
#ifdef KWS_TYPE
#if KWS_TYPE == DNN
//#include "kws_dnn.h"
#elif KWS_TYPE == DS_CNN
//#include "kws_ds_cnn.h"
#else
//#error "Invalid value of KWS_TYPE (Choose 'DNN' or 'DS_CNN')"
#endif /* KWS_TYPE */
//#error "KWS_TYPE not defined (Choose 'DNN' or 'DS_CNN')"
#endif /* KWS_TYPE */
#include "kws_f413zh.h"
#include "plot_utils.h"

#include "wav_data.h"
// TODO
//#include "audio_rec_dfsdm.h"

int16_t audio_buffer[16000]=WAVE_DATA;

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define HEADBAND_HEIGHT    64
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
TS_StateTypeDef TS_State;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static void myBSP_Init(void);
static void TF_HelloWorld_demo(void);
static void ML_KWS_demo(void);
static void ML_KWS_Live_demo(void);
// TODO
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
  myBSP_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	//AudioRecDfsdm_demo();
	//TF_HelloWorld_demo();
	//ML_KWS_demo();
	ML_KWS_Live_demo();
	//int8_t buffer[1];
	//HAL_UART_Receive(&huart6, buffer, sizeof(buffer), HAL_MAX_DELAY);
	//HAL_UART_Transmit(&huart6, buffer, sizeof(buffer), HAL_MAX_DELAY);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 200;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
uint8_t CheckForUserInput(void)
{
  if(BSP_PB_GetState(BUTTON_WAKEUP) != GPIO_PIN_RESET)
  {
    while (BSP_PB_GetState(BUTTON_WAKEUP) != GPIO_PIN_RESET);
    return 1 ;
  }
  return 0;
}

static void myBSP_Init(void)
{
  BSP_LCD_Init();
  BSP_LCD_DisplayOn();
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  //Touchscreen_Calibration();
  BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetFont(&Font12);
  char hello_str[20];
  sprintf(hello_str, "Hello World!");
  BSP_LCD_DisplayStringAt(0, 2, (uint8_t *)hello_str, CENTER_MODE);
}

/**
  * @brief  Display Audio Record demo hint
  * @param  None
  * @retval None
  */
static void HelloWorld_SetHint(void)
{
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set Audio Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), HEADBAND_HEIGHT);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAt(0, 1, (uint8_t *)"TFLITE SINE EXAMPLE", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 20, (uint8_t *)"Drawing 40 points", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 35, (uint8_t *)"on sine wave model", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 50, (uint8_t *)"Press BUTTON for next", CENTER_MODE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
}

/**
  * @brief  Display Audio Record demo hint
  * @param  None
  * @retval None
  */
static void KWS_SetHint(void)
{
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set Audio Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), HEADBAND_HEIGHT);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAt(0, 1, (uint8_t *)"ARM KWS EXAMPLE", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 20, (uint8_t *)"Detecting keywords", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 35, (uint8_t *)"from 1 second .wav file", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 50, (uint8_t *)"Press BUTTON for next", CENTER_MODE);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
}

static void TF_HelloWorld_demo()
{
  int counter = 0;
	circle_t *tmp_circle;
	int count = 0;
	uint16_t screen_height = BSP_LCD_GetYSize();
	uint16_t screen_width = BSP_LCD_GetXSize();
	uint16_t x_pos, y_pos;
  HelloWorld_SetHint();
	setup();
  for(;;) {
   	if (count < 40) {
	  	tmp_circle = loop();
	   	if (tmp_circle) {
	   		x_pos = (uint16_t)(tmp_circle->x * screen_width /
	   				   (2 * PI)) + tmp_circle->size/2;
	   		y_pos = (uint16_t)(HEADBAND_HEIGHT + ((screen_height-HEADBAND_HEIGHT) / 2) +
	   				   tmp_circle->y * (screen_height-HEADBAND_HEIGHT-tmp_circle->size*10) / 2);
	   		BSP_LCD_FillCircle(x_pos, y_pos, tmp_circle->size);
	   	}
	   	count++;
    } else {
      count = 0;
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      BSP_LCD_FillRect(0, HEADBAND_HEIGHT, BSP_LCD_GetXSize(), screen_height-HEADBAND_HEIGHT);
      BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
    }
    if (CheckForUserInput() > 0) {
      return;
    } else {
	   	HAL_Delay(100);
    }
  }
}

//static void ML_KWS_demo()
//{
//  uint32_t ticks1, ticks2;
//	uint16_t screen_height = BSP_LCD_GetYSize();
//	uint16_t screen_width = BSP_LCD_GetXSize();
//	uint16_t x_pos, y_pos;
//  KWS_SetHint();
//  char output_class[12][8] = {"Silence", "Unknown","yes","no","up","down","left","right","on","off","stop","go"};
//#ifdef KWS_TYPE
//#if KWS_TYPE == DNN
//  KWS_DNN kws(audio_buffer);
//#elif KWS_TYPE == DS_CNN
//  KWS_DS_CNN kws(audio_buffer);
//#else
////#error "Invalid value of KWS_TYPE (Choose 'DNN' or 'DS_CNN')"
//#endif /* KWS_TYPE */
////#error "KWS_TYPE not defined (Choose 'DNN' or 'DS_CNN')"
//#endif /* KWS_TYPE */
//  ticks1 = HAL_GetTick();
//  kws.extract_features(); //extract mfcc features
//  kws.classify();   //classify using dnn
//  ticks2 = HAL_GetTick();
//  int max_ind = kws.get_top_class(kws.output);
//  char kws_str[20];
//  sprintf(kws_str, "Total time : %d us",ticks2-ticks1);
//  BSP_LCD_DisplayStringAt(0, LINE(8), (uint8_t *)kws_str, CENTER_MODE);
//  sprintf(kws_str, "Detected %s (%d%%)",output_class[max_ind],((int)kws.output[max_ind]*100/128));
//  BSP_LCD_DisplayStringAt(0, LINE(10), (uint8_t *)kws_str, CENTER_MODE);
//  while (CheckForUserInput() == 0) {
//  	HAL_Delay(100);
//  }
//}

char lcd_output_string[64];
char ticks_output_string[64];
char output_class[12][8] = {"Silence", "Unknown","yes","no","up","down",
                            "left","right","on","off","stop","go"};
// Tune the following three parameters to improve the detection accuracy
//  and reduce false positives
// Longer averaging window and higher threshold reduce false positives
//  but increase detection latency and reduce true positive detections.

// (recording_win*frame_shift) is the actual recording window size
int recording_win = 3; 
// Averaging window for smoothing out the output predictions
int averaging_window_len = 3;  
int detection_threshold = 90;  //in percent
KWS_F413ZH *kws;

void run_kws();

/*
 * The audio recording works with two ping-pong buffers.
 * The data for each window will be tranfered by the DMA, which sends
 * sends an interrupt after the transfer is completed.
 */

// Manages the DMA Transfer complete interrupt.
void BSP_AUDIO_IN_TransferComplete_CallBack(void)
{
  arm_copy_q7((q7_t *)kws->audio_buffer_in + kws->audio_block_size*4, (q7_t *)kws->audio_buffer_out + kws->audio_block_size*4, kws->audio_block_size*4);
  if(kws->frame_len != kws->frame_shift) {
    //copy the last (frame_len - frame_shift) audio data to the start
    arm_copy_q7((q7_t *)(kws->audio_buffer)+2*(kws->audio_buffer_size-(kws->frame_len-kws->frame_shift)), (q7_t *)kws->audio_buffer, 2*(kws->frame_len-kws->frame_shift));
  }
  // copy the new recording data 
  for (int i=0;i<kws->audio_block_size;i++) {
    kws->audio_buffer[kws->frame_len-kws->frame_shift+i] = kws->audio_buffer_in[2*kws->audio_block_size+i*2];
  }
  run_kws();
  return;
}

// Manages the DMA Half Transfer complete interrupt.
void BSP_AUDIO_IN_HalfTransfer_CallBack(void)
{
  arm_copy_q7((q7_t *)kws->audio_buffer_in, (q7_t *)kws->audio_buffer_out, kws->audio_block_size*4);
  if(kws->frame_len!=kws->frame_shift) {
    //copy the last (frame_len - frame_shift) audio data to the start
    arm_copy_q7((q7_t *)(kws->audio_buffer)+2*(kws->audio_buffer_size-(kws->frame_len-kws->frame_shift)), (q7_t *)kws->audio_buffer, 2*(kws->frame_len-kws->frame_shift));
  }
  // copy the new recording data 
  for (int i=0;i<kws->audio_block_size;i++) {
    kws->audio_buffer[kws->frame_len-kws->frame_shift+i] = kws->audio_buffer_in[i*2];
  }
  run_kws();
  return;
}

void run_kws()
{
  static char kws_str1[20];
  char kws_str2[40];
  uint32_t ticks_before, ticks_after;
  ticks_before = HAL_GetTick();
  kws->extract_features();    //extract mfcc features
  kws->classify();	      //classify using dnn
  kws->average_predictions();
  //plot_mfcc();
  //plot_waveform();
  int max_ind = kws->get_top_class(kws->averaged_output);
  if(kws->averaged_output[max_ind]>detection_threshold*128/100)
    sprintf(kws_str1,"%d%% %s",((int)kws->averaged_output[max_ind]*100/128),output_class[max_ind]);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_ClearStringLine(8);
  BSP_LCD_DisplayStringAt(0, LINE(8), (uint8_t *)kws_str1, CENTER_MODE);
  ticks_after = HAL_GetTick();
  BSP_LCD_ClearStringLine(15);
  uint32_t clock = HAL_RCC_GetSysClockFreq();
  sprintf(kws_str2,"Ticks: %d ms @ %ld Hz",ticks_after-ticks_before, clock);
  BSP_LCD_DisplayStringAt(0, LINE(15), (uint8_t *)kws_str2, CENTER_MODE);
}


static void ML_KWS_Live_demo()
{
  uint32_t ticks1, ticks2;
	uint16_t screen_height = BSP_LCD_GetYSize();
	uint16_t screen_width = BSP_LCD_GetXSize();
	uint16_t x_pos, y_pos;
  //KWS_Live_SetHint();
  kws = new KWS_F413ZH(recording_win,averaging_window_len);
  init_plot();
  kws->start_kws();
  while (CheckForUserInput() == 0) {
  	HAL_Delay(100);
  }
  for(;;); // TODO
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
