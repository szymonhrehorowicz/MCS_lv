/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "can.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "errors.h"
#include "init.h"
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define SENS_RESISTOR 100U // It's an 10 mR resistor, but to calculate current we divide voltage by this value, so it's more convenient to multiply by 100 than divide by 10/1000
#define OPAMP_GAIN 20U
#define ADC_SUPPLY_VOLTAGE 3.3f
#define ADC_RANGE 4096U

// ADC buffer
uint32_t ADC_buffer[NUMBER_OF_USED_ADC_CHANNELS];

bool batteryPackState = ON;
uint8_t Read_MeasuredCurrent[2];
SetOfEntities voltages;
SetOfEntities temperatures;

void measureCurrent(void);
void measureEntities(MeasureSetup * setup);
void setMultiplexer(uint8_t cell, MultiplexerSetup multiplexer);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	MultiplexerSetup voltageMultiplexer, temperatureMultiplexer;
	MeasureSetup voltageMeasure, temperatureMeasure;

	multiplexersInit(&voltageMultiplexer, &temperatureMultiplexer);
	measureEntitiesInit(&voltageMeasure, &temperatureMeasure, &voltages, &temperatures);

	voltageMeasure.multiplexer = voltageMultiplexer;
	temperatureMeasure.multiplexer = temperatureMultiplexer;


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
  MX_DMA_Init();
  MX_ADC_Init();
  MX_USART1_UART_Init();
  MX_CAN_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  measureCurrent();
	  measureEntities(&voltageMeasure);
	  measureEntities(&temperatureMeasure);

	  if(batteryPackState == ON) {
		  HAL_GPIO_WritePin(V_I_T_OK_GPIO_Port, V_I_T_OK_Pin, GPIO_PIN_RESET);
	  }else {
		  HAL_GPIO_WritePin(V_I_T_OK_GPIO_Port, V_I_T_OK_Pin, GPIO_PIN_SET);
	  }
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void measureCurrent(void) {
	uint32_t value = ADC_buffer[CURRENT_CHANNEL];

	uint32_t current = (ADC_SUPPLY_VOLTAGE * value * SENS_RESISTOR) / (ADC_RANGE * OPAMP_GAIN);

	Read_MeasuredCurrent[MOST_SIGNIFICANT_BYTE] = (current >> SHIFT_BYTE);
	Read_MeasuredCurrent[LESS_SIGNIFICANT_BYTE] = current;

	bool isOverLimit = current > CURRENT_MAX_VALUE;

	if(isOverLimit) OvercurrentError_Handler();
}

void measureEntities(MeasureSetup * setup) {
	HAL_GPIO_WritePin(setup->startPort, setup->startPin, GPIO_PIN_RESET);

	for(uint8_t measuredCell = setup->firstCellToMeasure; measuredCell <= setup->lastCellToMeasure; measuredCell++) {
		uint32_t value;

		setMultiplexer(measuredCell, setup->multiplexer);
		value = ADC_buffer[setup->ADC_Channel];

		// TODO: Calculate real voltage

		setup->entities->values[measuredCell].value[MOST_SIGNIFICANT_BYTE] = (value >> SHIFT_BYTE);
		setup->entities->values[measuredCell].value[LESS_SIGNIFICANT_BYTE] = value;

		bool isOverLimit = value > setup->maxValue;
		bool isUnderLimit = value < setup->minValue;

		if(isOverLimit) setup->overBoundaryError();
		if(isUnderLimit) setup->underBoundaryError();
	}
    // TODO: Pytanie, czy sprawdzać każde celle czy poza limitem czy tylko średnią?
	if(setup->multiplexer.type == VOLTAGE_MULTIPLEXER_TYPE) {
		// TODO: Measure avg voltage
	}

	HAL_GPIO_WritePin(setup->startPort, setup->startPin, GPIO_PIN_SET);
}

void setMultiplexer(uint8_t cell, MultiplexerSetup multiplexer) {
	if(cell & multiplexer.maskA) {
		HAL_GPIO_WritePin(multiplexer.portA, multiplexer.pinA, GPIO_PIN_SET);
	}else {
		HAL_GPIO_WritePin(multiplexer.portA, multiplexer.pinA, GPIO_PIN_SET);
	}

	if(cell & multiplexer.maskB) {
		HAL_GPIO_WritePin(multiplexer.portB, multiplexer.pinB, GPIO_PIN_SET);
	}else {
		HAL_GPIO_WritePin(multiplexer.portB, multiplexer.pinB, GPIO_PIN_SET);
	}

	if(cell & multiplexer.maskC) {
		HAL_GPIO_WritePin(multiplexer.portC, multiplexer.pinC, GPIO_PIN_SET);
	}else {
		HAL_GPIO_WritePin(multiplexer.portC, multiplexer.pinC, GPIO_PIN_SET);
	}

	if(multiplexer.type == TEMPERATURE_MULTIPLEXER_TYPE) {
		if(cell & multiplexer.maskD) {
			HAL_GPIO_WritePin(multiplexer.portD, multiplexer.pinD, GPIO_PIN_SET);
		}else {
			HAL_GPIO_WritePin(multiplexer.portD, multiplexer.pinD, GPIO_PIN_SET);
		}
	}else{
		HAL_Delay(TRANSISTORS_DELAY);
	}
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
  __disable_irq();
  while (1)
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
