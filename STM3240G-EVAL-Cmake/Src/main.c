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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "color.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
osThreadId defaultTaskHandle;
osThreadId blinkTaskHandleHandle;
/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void StartDefaultTask(void const *argument);
void blink(void const *argument);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

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
	char *hello_str = "Hello World";
	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */
	/* Initialize the LCD */
	BSP_LCD_Init();

	/* Enable the LCD */
	BSP_LCD_DisplayOn();

	/* Clear the LCD Background layer */
	BSP_LCD_Clear(LCD_COLOR_WHITE);

	Touchscreen_Calibration();
	BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());

	BSP_LCD_Clear(LCD_COLOR_WHITE);
	BSP_LCD_SetTextColor(LCD_COLOR_DARKRED);
	BSP_LCD_SetFont(&Font8);
	BSP_LCD_DisplayStringAt(210, (BSP_LCD_GetYSize() - 55),
				(uint8_t *)hello_str, LEFT_MODE);
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

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

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

	/* Start scheduler */
	osKernelStart();

	/* We should never get here as control is now taken by the scheduler */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
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
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage 
  */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/** Initializes the CPU, AHB and APB busses clocks 
  */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 168;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks 
  */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK |
				      RCC_CLOCKTYPE_SYSCLK |
				      RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) !=
	    HAL_OK) {
		Error_Handler();
	}
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

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
	HAL_GPIO_WritePin(GPIOG, SmartCard_CMDVCC_Pin | LED2_Pin | LED1_Pin,
			  GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOH, SmartCard_3_5V_Pin | OTG_FS_PowerSwitchOn_Pin,
			  GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(SmartCard_RST_GPIO_Port, SmartCard_RST_Pin,
			  GPIO_PIN_RESET);

	/*Configure GPIO pins : A19_Pin FSMC_NBL1_Pin FSMC_NBL0_Pin A20_Pin 
                           D10_Pin D5_Pin D6_Pin D8_Pin 
                           D11_Pin D4_Pin D7_Pin D9_Pin 
                           D12_Pin */
	GPIO_InitStruct.Pin = A19_Pin | FSMC_NBL1_Pin | FSMC_NBL0_Pin |
			      A20_Pin | D10_Pin | D5_Pin | D6_Pin | D8_Pin |
			      D11_Pin | D4_Pin | D7_Pin | D9_Pin | D12_Pin;
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
	GPIO_InitStruct.Pin = ULPI_D7_Pin | ULPI_D5_Pin | ULPI_D6_Pin |
			      ULPI_D2_Pin | ULPI_D1_Pin | ULPI_D3_Pin |
			      ULPI_D4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : MII_TXD1_Pin MII_TXD0_Pin MII_TX_EN_Pin */
	GPIO_InitStruct.Pin = MII_TXD1_Pin | MII_TXD0_Pin | MII_TX_EN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	/*Configure GPIO pins : FSMC_NE1_Pin FSMC_NWAIT_Pin D2_Pin FSMC_NWE_Pin 
                           D3_Pin FSMC_NOE_Pin FSMC_CLK_Pin D1_Pin 
                           D0_Pin A18_Pin A17_Pin A16_Pin 
                           D15_Pin D14_Pin D13_Pin */
	GPIO_InitStruct.Pin = FSMC_NE1_Pin | FSMC_NWAIT_Pin | D2_Pin |
			      FSMC_NWE_Pin | D3_Pin | FSMC_NOE_Pin |
			      FSMC_CLK_Pin | D1_Pin | D0_Pin | A18_Pin |
			      A17_Pin | A16_Pin | D15_Pin | D14_Pin | D13_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/*Configure GPIO pins : MicroSDCard_CLK_Pin MicroSDCard_D1_Pin MicroSDCard_D0_Pin */
	GPIO_InitStruct.Pin =
		MicroSDCard_CLK_Pin | MicroSDCard_D1_Pin | MicroSDCard_D0_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : I2C1_SDA_Pin I2C1_SCL_Pin */
	GPIO_InitStruct.Pin = I2C1_SDA_Pin | I2C1_SCL_Pin;
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
	GPIO_InitStruct.Pin = SmartCard_CMDVCC_Pin | LED2_Pin | LED1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	/*Configure GPIO pins : FSMC_NE3_Pin FSMC_NE2_Pin A15_Pin A14_Pin 
                           A13_Pin A12_Pin A11_Pin A10_Pin */
	GPIO_InitStruct.Pin = FSMC_NE3_Pin | FSMC_NE2_Pin | A15_Pin | A14_Pin |
			      A13_Pin | A12_Pin | A11_Pin | A10_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	/*Configure GPIO pins : MicroSDCard_D3_Pin MicroSDCard_D2_Pin */
	GPIO_InitStruct.Pin = MicroSDCard_D3_Pin | MicroSDCard_D2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : USB_FS_DP_Pin USB_FS_DM_Pin USB_FS_ID_Pin */
	GPIO_InitStruct.Pin = USB_FS_DP_Pin | USB_FS_DM_Pin | USB_FS_ID_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : DCMI_D7_Pin DCMI_D6_Pin DCMI_VSYNC_Pin DCMI_D5_Pin */
	GPIO_InitStruct.Pin =
		DCMI_D7_Pin | DCMI_D6_Pin | DCMI_VSYNC_Pin | DCMI_D5_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
	HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

	/*Configure GPIO pins : I2S_SD_Pin I2S_WS_Pin */
	GPIO_InitStruct.Pin = I2S_SD_Pin | I2S_WS_Pin;
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
	GPIO_InitStruct.Pin = SmartCard_3_5V_Pin | OTG_FS_PowerSwitchOn_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

	/*Configure GPIO pins : A0_Pin A2_Pin A1_Pin A3_Pin 
                           A4_Pin A5_Pin A7_Pin A6_Pin 
                           A9_Pin A8_Pin */
	GPIO_InitStruct.Pin = A0_Pin | A2_Pin | A1_Pin | A3_Pin | A4_Pin |
			      A5_Pin | A7_Pin | A6_Pin | A9_Pin | A8_Pin;
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
	GPIO_InitStruct.Pin = DCMI_D4_Pin | DCMI_D3_Pin | DCMI_D2_Pin |
			      DCMI_D1_Pin | DCMI_HSYNC_Pin | DCMI_D0_Pin;
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
	GPIO_InitStruct.Pin =
		MII_CRS_Pin | MII_COL_Pin | MII_RXD2_Pin | MII_RXD3_Pin;
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
	GPIO_InitStruct.Pin = Audio_IN_Pin | Potentiometer_Pin;
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
	GPIO_InitStruct.Pin = MII_MDC_Pin | MII_TXD2_Pin | MII_TX_CLK_Pin |
			      MII_RXD0_Pin | MII_RXD1_Pin;
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
	GPIO_InitStruct.Pin = MII_RX_CLK_RMII_REF_CLK_Pin | MII_MDIO_Pin |
			      MII_RX_DV_RMII_CRSDV_Pin;
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
	GPIO_InitStruct.Pin = ULPI_CLK_Pin | ULPI_D0_Pin;
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
void StartDefaultTask(void const *argument)
{
	/* USER CODE BEGIN 5 */
	/* Infinite loop */
	for (;;) {
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
void blink(void const *argument)
{
	/* USER CODE BEGIN blink */
	/* Infinite loop */
	int count = 0;

	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);

	for (;;) {
		count %= 10;

		switch (count++) {
		case 0:
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin,
					  GPIO_PIN_SET);
			break;
		case 1:
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin,
					  GPIO_PIN_SET);
			break;
		case 2:
			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin,
					  GPIO_PIN_SET);
			break;
		case 3:
			HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin,
					  GPIO_PIN_SET);
			break;
		case 5:
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin,
					  GPIO_PIN_RESET);
			break;
		case 6:
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin,
					  GPIO_PIN_RESET);
			break;
		case 7:
			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin,
					  GPIO_PIN_RESET);
			break;
		case 8:
			HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin,
					  GPIO_PIN_RESET);
			break;
		default:
			break;
		}

		osDelay(100);
	}
	/* USER CODE END blink */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
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
