/**
  ******************************************************************************
  * @file           : main.cpp
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * TODO: License
  *
  ******************************************************************************
  */

/* STM32Cube Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"


/* TFLM Includes ------------------------------------------------------------------*/
//#include "main_functions.h"
#include "test_micro_speech.h"

/* Private includes ----------------------------------------------------------*/
#include <string.h>
#include <cstdio>

#ifdef BENCHMARKING
#include "benchmarking.h"
#endif /* BENCHMARKING */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define HEADBAND_HEIGHT    48
#define UartHandle huart6

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
//TS_StateTypeDef TS_State;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void BSP_Init(void);
static void BSP_Welcome(void);

/* Private user code ---------------------------------------------------------*/

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Local Variables */
#ifdef BENCHMARKING
  int divider = 0;
#endif /* BENCHMARKING */

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART6_UART_Init();

  /* Setup BSP Components */
  BSP_Init();

  /* Show Welcome Screen */
  BSP_Welcome();

  /* Infinite loop */
  fprintf(stderr, "--- TFLM Demo for STM32 Boards ---\n\r");
  //setup();
  test_setup();
  fprintf(stderr, "Setup done! Main loop starts now!\n\r");
  while (true) {
    //loop();
    test_loop();
    HAL_Delay(1000);
#ifdef BENCHMARKING
    if (divider % 10 == 0) {
//print_summary(TICKS_POPULATE|TICKS_INVOKE|TICKS_RESPOND);
    }
    divider++;
#endif /* BENCHMARKING */
  }

  return 0;
}

/**
  * @brief System Clock Configuration
  * @retval None
  *
  * TODO: Add Clock Values
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks */
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
  /** Initializes the CPU, AHB and APB busses clocks */
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

/**
  * @brief System Clock Configuration
  * @retval uint8_t (1 when button clicked)
  *
  */
uint8_t CheckForUserInput(void)
{
  if(BSP_PB_GetState(BUTTON_WAKEUP) != GPIO_PIN_RESET)
  {
    while (BSP_PB_GetState(BUTTON_WAKEUP) != GPIO_PIN_RESET);
    return 1 ;
  }
  return 0;
}

/**
  * @brief Board Support Package Initialization
  * @retval None
  *
  */
static void BSP_Init(void)
{
  /* Configure LCD */
  BSP_LCD_Init();
  BSP_LCD_DisplayOn();
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Configure Touchscreen (optional) */
  //Touchscreen_Calibration();
  //BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());

  /* Set Default LCD Colors and Fonts */
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetFont(&Font12);
}

/**
  * @brief Show Welcome Message on LCD
  * @retval None
  *
  */
static void BSP_Welcome(void)
{
  /* Local Variables */
  const uint16_t delay_ms = 1000;

  /* Save old Display Configuration */
  sFONT* old_font = BSP_LCD_GetFont();
  uint32_t old_text_color = BSP_LCD_GetTextColor();
  uint32_t old_back_color = BSP_LCD_GetTextColor();
  char str[32];

  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Draw Banner */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), HEADBAND_HEIGHT);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);

  /* Add Text to banner */
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAt(0, 5, (uint8_t *)"TFLM Demo", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 25, (uint8_t *)"for STM32 Boards", CENTER_MODE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  sprintf(str, "Board: %16s", TFLM_BOARD);
  BSP_LCD_DisplayStringAt(0, 100, (uint8_t *)str, CENTER_MODE);
  sprintf(str, "EXAMPLE: %16s", TFLM_EXAMPLE);
  BSP_LCD_DisplayStringAt(0, 150, (uint8_t *)str, CENTER_MODE);

  /* Wait a moment */
  HAL_Delay(delay_ms);

  /* Restore Font and Colors*/
  BSP_LCD_SetTextColor(old_text_color);
  BSP_LCD_SetBackColor(old_back_color);
  BSP_LCD_SetFont(old_font);
}

/**
  * @brief  Function used by printf tto write to the serial terminal
  * @retval int
  */
int __io_putchar(int ch)
{
  HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */
  fprintf(stderr, "Error_Handler called! Waiting for reset...\n\r");
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
