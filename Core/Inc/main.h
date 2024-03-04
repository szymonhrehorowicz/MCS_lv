/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define V_I_T_OK_Pin GPIO_PIN_0
#define V_I_T_OK_GPIO_Port GPIOA
#define MEASURED_CELL_CURRENT_Pin GPIO_PIN_1
#define MEASURED_CELL_CURRENT_GPIO_Port GPIOA
#define MEASURED_CELL_VOLTAGE_Pin GPIO_PIN_2
#define MEASURED_CELL_VOLTAGE_GPIO_Port GPIOA
#define uC_PWM_Pin GPIO_PIN_3
#define uC_PWM_GPIO_Port GPIOA
#define SAMPLE_Pin GPIO_PIN_4
#define SAMPLE_GPIO_Port GPIOA
#define DISCHARGE_Pin GPIO_PIN_5
#define DISCHARGE_GPIO_Port GPIOA
#define uC_C_Pin GPIO_PIN_6
#define uC_C_GPIO_Port GPIOA
#define uC_B_Pin GPIO_PIN_7
#define uC_B_GPIO_Port GPIOA
#define uC_A_Pin GPIO_PIN_0
#define uC_A_GPIO_Port GPIOB
#define MEASURED_TEMPERATURE_Pin GPIO_PIN_1
#define MEASURED_TEMPERATURE_GPIO_Port GPIOB
#define uC_INH_Pin GPIO_PIN_2
#define uC_INH_GPIO_Port GPIOB
#define PCB_LED_RED_Pin GPIO_PIN_10
#define PCB_LED_RED_GPIO_Port GPIOB
#define PCB_LED_GREEN_Pin GPIO_PIN_11
#define PCB_LED_GREEN_GPIO_Port GPIOB
#define uC_S3_Pin GPIO_PIN_12
#define uC_S3_GPIO_Port GPIOB
#define uC_S2_Pin GPIO_PIN_13
#define uC_S2_GPIO_Port GPIOB
#define uC_E_Pin GPIO_PIN_14
#define uC_E_GPIO_Port GPIOB
#define uC_S1_Pin GPIO_PIN_15
#define uC_S1_GPIO_Port GPIOB
#define uC_S0_Pin GPIO_PIN_8
#define uC_S0_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */
#define CURRENT_MAX_VALUE 11U
#define VOLTAGE_MAX_VALUE 4095	   // TODO: define experimentally
#define VOLTAGE_MIN_VALUE 0		   // TODO: define experimentally
#define TEMPERATURE_MAX_VALUE 4095 // TODO: define experimentally
#define TEMPERATURE_MIN_VALUE 0
#define TRANSISTORS_DELAY 10	   // TODO: define experimentally
#define TEMPERATURE_MULTIPLEXER_TYPE true
#define VOLTAGE_MULTIPLEXER_TYPE false
#define ON true
#define NUMBER_OF_USED_ADC_CHANNELS 3U

enum
{
	BAT_1m = 0b00000000U,
	BAT_6m = 0b00000101U,
	UC_A_MASK = 0b00000001U,
	UC_B_MASK = 0b00000010U,
	UC_C_MASK = 0b00000100U,
	TEMP_1m = 0b00000000U,
	TEMP_16m = 0b00001111U,
	UC_S0_MASK = 0b00000001U,
	UC_S1_MASK = 0b00000010U,
	UC_S2_MASK = 0b00000100U,
	UC_S3_MASK = 0b00001000U,
};

enum
{
	CURRENT_CHANNEL,
	VOLTAGE_CHANNEL,
	TEMPEARATURE_CHANNEL,
};

enum
{
	MOST_SIGNIFICANT_BYTE = 0U,
	LESS_SIGNIFICANT_BYTE = 1U,
	SHIFT_BYTE = 8U,
};

typedef struct entity
{
	uint8_t value[2];
}Entity;

typedef struct setOfEntities
{
	uint8_t overallPercentageValue[2];
	Entity values[6];
}SetOfEntities;

typedef struct multiplexerSetup
{
	bool type;
	GPIO_TypeDef *portA;
	uint16_t pinA;
	uint8_t maskA;
	GPIO_TypeDef *portB;
	uint16_t pinB;
	uint8_t maskB;
	GPIO_TypeDef *portC;
	uint16_t pinC;
	uint8_t maskC;
	GPIO_TypeDef *portD;
	uint16_t pinD;
	uint8_t maskD;
} MultiplexerSetup;

typedef struct measureSetup
{
	GPIO_TypeDef *startPort;
	uint16_t startPin;
	uint8_t firstCellToMeasure;
	uint8_t lastCellToMeasure;
	uint8_t ADC_Channel;
	uint32_t maxValue;
	uint32_t minValue;
	SetOfEntities* entities;
	MultiplexerSetup multiplexer;
	void (*overBoundaryError)(void);
	void (*underBoundaryError)(void);
} MeasureSetup;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
