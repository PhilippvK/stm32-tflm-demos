
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2020 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "libjpeg.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
osThreadId defaultTaskHandle;
osThreadId blinkTaskHandleHandle;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void StartDefaultTask(void const * argument);
void blink(void const * argument);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of blinkTaskHandle */
  osThreadDef(blinkTaskHandle, blink, osPriorityIdle, 0, 128);
  blinkTaskHandleHandle = osThreadCreate(osThread(blinkTaskHandle), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
 

  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

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

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

/** Configure pins
     PE3   ------> FSMC_A19
     PE1   ------> FSMC_NBL1
     PE0   ------> FSMC_NBL0
     PB8   ------> ETH_TXD3
     PB5   ------> USB_OTG_HS_ULPI_D7
     PG14   ------> ETH_TXD1
     PG13   ------> ETH_TXD0
     PD7   ------> FSMC_NE1
     PC12   ------> SDIO_CK
     PE4   ------> FSMC_A20
     PB9   ------> I2C1_SDA
     PB7   ------> FSMC_NL
     PB6   ------> I2C1_SCL
     PG11   ------> ETH_TX_EN
     PG10   ------> FSMC_NE3
     PD6   ------> FSMC_NWAIT
     PD0   ------> FSMC_D2_DA2
     PC11   ------> USART3_RX
     PC10   ------> USART3_TX
     PA12   ------> USB_OTG_FS_DP
     PI7   ------> DCMI_D7
     PI6   ------> DCMI_D6
     PI5   ------> DCMI_VSYNC
     PG9   ------> FSMC_NE2
     PD5   ------> FSMC_NWE
     PD1   ------> FSMC_D3_DA3
     PI3   ------> I2S2_SD
     PA11   ------> USB_OTG_FS_DM
     PI4   ------> DCMI_D5
     PD4   ------> FSMC_NOE
     PD3   ------> FSMC_CLK
     PD2   ------> SDIO_CMD
     PA10   ------> USB_OTG_FS_ID
     PF0   ------> FSMC_A0
     PI10   ------> ETH_RX_ER
     PI11   ------> USB_OTG_HS_ULPI_DIR
     PH14   ------> DCMI_D4
     PI0   ------> I2S2_WS
     PA9   ------> USB_OTG_FS_VBUS
     PH2   ------> ETH_CRS
     PC9   ------> SDIO_D1
     PA8   ------> RCC_MCO_1
     PH3   ------> ETH_COL
     PC8   ------> SDIO_D0
     PF2   ------> FSMC_A2
     PF1   ------> FSMC_A1
     PH4   ------> USB_OTG_HS_ULPI_NXT
     PF3   ------> FSMC_A3
     PF4   ------> FSMC_A4
     PG7   ------> USART6_CK
     PF5   ------> FSMC_A5
     PH12   ------> DCMI_D3
     PG5   ------> FSMC_A15
     PG4   ------> FSMC_A14
     PG3   ------> FSMC_A13
     PF10   ------> ADC3_IN8
     PF9   ------> ADC3_IN7
     PH11   ------> DCMI_D2
     PH10   ------> DCMI_D1
     PD15   ------> FSMC_D1_DA1
     PG2   ------> FSMC_A12
     PC0   ------> USB_OTG_HS_ULPI_STP
     PC1   ------> ETH_MDC
     PC2   ------> ETH_TXD2
     PC3   ------> ETH_TX_CLK
     PG1   ------> FSMC_A11
     PH6   ------> ETH_RXD2
     PH8   ------> DCMI_HSYNC
     PH9   ------> DCMI_D0
     PD14   ------> FSMC_D0_DA0
     PD13   ------> FSMC_A18
     PA1   ------> ETH_RX_CLK
     PA4   ------> COMP_DAC1_group
     PC4   ------> ETH_RXD0
     PF13   ------> FSMC_A7
     PG0   ------> FSMC_A10
     PE13   ------> FSMC_D10_DA10
     PH7   ------> ETH_RXD3
     PD12   ------> FSMC_A17_ALE
     PD11   ------> FSMC_A16_CLE
     PD10   ------> FSMC_D15_DA15
     PA2   ------> ETH_MDIO
     PA6   ------> DCMI_PIXCK
     PA5   ------> USB_OTG_HS_ULPI_CK
     PC5   ------> ETH_RXD1
     PF12   ------> FSMC_A6
     PF15   ------> FSMC_A9
     PE8   ------> FSMC_D5_DA5
     PE9   ------> FSMC_D6_DA6
     PE11   ------> FSMC_D8_DA8
     PE14   ------> FSMC_D11_DA11
     PB12   ------> USB_OTG_HS_ULPI_D5
     PB13   ------> USB_OTG_HS_ULPI_D6
     PD9   ------> FSMC_D14_DA14
     PD8   ------> FSMC_D13_DA13
     PA3   ------> USB_OTG_HS_ULPI_D0
     PA7   ------> ETH_RX_DV
     PB1   ------> USB_OTG_HS_ULPI_D2
     PB0   ------> USB_OTG_HS_ULPI_D1
     PF14   ------> FSMC_A8
     PE7   ------> FSMC_D4_DA4
     PE10   ------> FSMC_D7_DA7
     PE12   ------> FSMC_D9_DA9
     PE15   ------> FSMC_D12_DA12
     PB10   ------> USB_OTG_HS_ULPI_D3
     PB11   ------> USB_OTG_HS_ULPI_D4
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, SmartCard_CMDVCC_Pin|LED2_Pin|LED1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOH, SmartCard_3_5V_Pin|OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SmartCard_RST_GPIO_Port, SmartCard_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : A19_Pin FSMC_NBL1_Pin FSMC_NBL0_Pin A20_Pin 
                           D10_Pin D5_Pin D6_Pin D8_Pin 
                           D11_Pin D4_Pin D7_Pin D9_Pin 
                           D12_Pin */
  GPIO_InitStruct.Pin = A19_Pin|FSMC_NBL1_Pin|FSMC_NBL0_Pin|A20_Pin 
                          |D10_Pin|D5_Pin|D6_Pin|D8_Pin 
                          |D11_Pin|D4_Pin|D7_Pin|D9_Pin 
                          |D12_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : MII_TXD3_Pin */
  GPIO_InitStruct.Pin = MII_TXD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(MII_TXD3_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ULPI_D7_Pin ULPI_D5_Pin ULPI_D6_Pin ULPI_D2_Pin 
                           ULPI_D1_Pin ULPI_D3_Pin ULPI_D4_Pin */
  GPIO_InitStruct.Pin = ULPI_D7_Pin|ULPI_D5_Pin|ULPI_D6_Pin|ULPI_D2_Pin 
                          |ULPI_D1_Pin|ULPI_D3_Pin|ULPI_D4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : MII_TXD1_Pin MII_TXD0_Pin MII_TX_EN_Pin */
  GPIO_InitStruct.Pin = MII_TXD1_Pin|MII_TXD0_Pin|MII_TX_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : FSMC_NE1_Pin FSMC_NWAIT_Pin D2_Pin FSMC_NWE_Pin 
                           D3_Pin FSMC_NOE_Pin FSMC_CLK_Pin D1_Pin 
                           D0_Pin A18_Pin A17_Pin A16_Pin 
                           D15_Pin D14_Pin D13_Pin */
  GPIO_InitStruct.Pin = FSMC_NE1_Pin|FSMC_NWAIT_Pin|D2_Pin|FSMC_NWE_Pin 
                          |D3_Pin|FSMC_NOE_Pin|FSMC_CLK_Pin|D1_Pin 
                          |D0_Pin|A18_Pin|A17_Pin|A16_Pin 
                          |D15_Pin|D14_Pin|D13_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : MicroSDCard_CLK_Pin MicroSDCard_D1_Pin MicroSDCard_D0_Pin */
  GPIO_InitStruct.Pin = MicroSDCard_CLK_Pin|MicroSDCard_D1_Pin|MicroSDCard_D0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : I2C1_SDA_Pin I2C1_SCL_Pin */
  GPIO_InitStruct.Pin = I2C1_SDA_Pin|I2C1_SCL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : FSMC_NL_Pin */
  GPIO_InitStruct.Pin = FSMC_NL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
  HAL_GPIO_Init(FSMC_NL_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : User_Button_Pin */
  GPIO_InitStruct.Pin = User_Button_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(User_Button_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SmartCard_CMDVCC_Pin LED2_Pin LED1_Pin */
  GPIO_InitStruct.Pin = SmartCard_CMDVCC_Pin|LED2_Pin|LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : FSMC_NE3_Pin FSMC_NE2_Pin A15_Pin A14_Pin 
                           A13_Pin A12_Pin A11_Pin A10_Pin */
  GPIO_InitStruct.Pin = FSMC_NE3_Pin|FSMC_NE2_Pin|A15_Pin|A14_Pin 
                          |A13_Pin|A12_Pin|A11_Pin|A10_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : MicroSDCard_D3_Pin MicroSDCard_D2_Pin */
  GPIO_InitStruct.Pin = MicroSDCard_D3_Pin|MicroSDCard_D2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : USB_FS_DP_Pin USB_FS_DM_Pin USB_FS_ID_Pin */
  GPIO_InitStruct.Pin = USB_FS_DP_Pin|USB_FS_DM_Pin|USB_FS_ID_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : DCMI_D7_Pin DCMI_D6_Pin DCMI_VSYNC_Pin DCMI_D5_Pin */
  GPIO_InitStruct.Pin = DCMI_D7_Pin|DCMI_D6_Pin|DCMI_VSYNC_Pin|DCMI_D5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /*Configure GPIO pins : I2S_SD_Pin I2S_WS_Pin */
  GPIO_InitStruct.Pin = I2S_SD_Pin|I2S_WS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /*Configure GPIO pin : IO_Expander_INT_Pin */
  GPIO_InitStruct.Pin = IO_Expander_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(IO_Expander_INT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED3_Pin */
  GPIO_InitStruct.Pin = LED3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED3_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MicroSDCard_CMD_Pin */
  GPIO_InitStruct.Pin = MicroSDCard_CMD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
  HAL_GPIO_Init(MicroSDCard_CMD_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SmartCard_3_5V_Pin OTG_FS_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = SmartCard_3_5V_Pin|OTG_FS_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pins : A0_Pin A2_Pin A1_Pin A3_Pin 
                           A4_Pin A5_Pin A7_Pin A6_Pin 
                           A9_Pin A8_Pin */
  GPIO_InitStruct.Pin = A0_Pin|A2_Pin|A1_Pin|A3_Pin 
                          |A4_Pin|A5_Pin|A7_Pin|A6_Pin 
                          |A9_Pin|A8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : MII_RX_ER_Pin */
  GPIO_InitStruct.Pin = MII_RX_ER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(MII_RX_ER_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ULPI_DIR_Pin */
  GPIO_InitStruct.Pin = ULPI_DIR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
  HAL_GPIO_Init(ULPI_DIR_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MicroSDCard_Detect_Pin */
  GPIO_InitStruct.Pin = MicroSDCard_Detect_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(MicroSDCard_Detect_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : DCMI_D4_Pin DCMI_D3_Pin DCMI_D2_Pin DCMI_D1_Pin 
                           DCMI_HSYNC_Pin DCMI_D0_Pin */
  GPIO_InitStruct.Pin = DCMI_D4_Pin|DCMI_D3_Pin|DCMI_D2_Pin|DCMI_D1_Pin 
                          |DCMI_HSYNC_Pin|DCMI_D0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pin : VBUS_FS_Pin */
  GPIO_InitStruct.Pin = VBUS_FS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(VBUS_FS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : MII_CRS_Pin MII_COL_Pin MII_RXD2_Pin MII_RXD3_Pin */
  GPIO_InitStruct.Pin = MII_CRS_Pin|MII_COL_Pin|MII_RXD2_Pin|MII_RXD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pin : MCO_Pin */
  GPIO_InitStruct.Pin = MCO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
  HAL_GPIO_Init(MCO_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED4_Pin */
  GPIO_InitStruct.Pin = LED4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED4_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ULPI_NXT_Pin */
  GPIO_InitStruct.Pin = ULPI_NXT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
  HAL_GPIO_Init(ULPI_NXT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SmartCard_CLK_Pin */
  GPIO_InitStruct.Pin = SmartCard_CLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
  HAL_GPIO_Init(SmartCard_CLK_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SmartCard_RST_Pin */
  GPIO_InitStruct.Pin = SmartCard_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SmartCard_RST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SmartCard_OFF_Pin */
  GPIO_InitStruct.Pin = SmartCard_OFF_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SmartCard_OFF_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Audio_IN_Pin Potentiometer_Pin */
  GPIO_InitStruct.Pin = Audio_IN_Pin|Potentiometer_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : ULPI_STP_Pin */
  GPIO_InitStruct.Pin = ULPI_STP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
  HAL_GPIO_Init(ULPI_STP_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : MII_MDC_Pin MII_TXD2_Pin MII_TX_CLK_Pin MII_RXD0_Pin 
                           MII_RXD1_Pin */
  GPIO_InitStruct.Pin = MII_MDC_Pin|MII_TXD2_Pin|MII_TX_CLK_Pin|MII_RXD0_Pin 
                          |MII_RXD1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : SW1_Pin */
  GPIO_InitStruct.Pin = SW1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SW1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : MII_RX_CLK_RMII_REF_CLK_Pin MII_MDIO_Pin MII_RX_DV_RMII_CRSDV_Pin */
  GPIO_InitStruct.Pin = MII_RX_CLK_RMII_REF_CLK_Pin|MII_MDIO_Pin|MII_RX_DV_RMII_CRSDV_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : Audio_DAC_OUT_Pin */
  GPIO_InitStruct.Pin = Audio_DAC_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Audio_DAC_OUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DCMI_PIXCK_Pin */
  GPIO_InitStruct.Pin = DCMI_PIXCK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
  HAL_GPIO_Init(DCMI_PIXCK_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ULPI_CLK_Pin ULPI_D0_Pin */
  GPIO_InitStruct.Pin = ULPI_CLK_Pin|ULPI_D0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_OverCurrent_Pin */
  GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MII_INT_Pin */
  GPIO_InitStruct.Pin = MII_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MII_INT_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for LIBJPEG */
  MX_LIBJPEG_Init();

  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */ 
}

/* USER CODE BEGIN Header_blink */
/**
* @brief Function implementing the blinkTaskHandle thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_blink */
void blink(void const * argument)
{
  /* USER CODE BEGIN blink */
  /* Infinite loop */
	int count = 0;

	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);


  for(;;)
  {
	  count %= 10;

	  switch(count++){
	  case 0:
		  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
		  break;
	  case 1:
		  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
		  break;
	  case 2:
		  HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
		  break;
	  case 3:
		  HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);
		  break;
	  case 5:
		  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
		  break;
	  case 6:
		  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
		  break;
	  case 7:
		  HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
		  break;
	  case 8:
		  HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
		  break;
	  default: break;
	  }

    osDelay(100);
  }
  /* USER CODE END blink */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
