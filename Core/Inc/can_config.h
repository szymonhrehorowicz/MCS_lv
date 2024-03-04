/*
 * can_config.h
 *
 *  Created on: 27.02.2022
 * Modified on: 30.07.2022
 *      Author: Krystian Sosin
 *     Version: 1.0.1
 * Last change: Fix minor bugs and add CAN_AcknowledgeWriteMessage() function.
 */

#ifndef INC_CAN_CONFIG_H_
#define INC_CAN_CONFIG_H_

#include "main.h"
#include "can.h"
#include "stdbool.h"

/* ---------------------------------------------------------------------------------------- */
/* -------------------------------START OF DEFINES----------------------------------------- */
/* ---------------------------------------------------------------------------------------- */

/* USER CODE BEGIN DEFINES */
#define NUMBER_OF_READ_REGS (10U)
#define NUMBER_OF_WRITE_REGS (1U)
#define NUMBER_OF_ERROR_REGS (4U)
/* USER CODE END DEFINES */

#define ERROR_DLC (2U)
#define ACKNOWLEDMENT_DLC (2U)
#define FIRST_ARRAY_ELEMENT (0U)

/* ---------------------------------------------------------------------------------------- */
/* -------------------------------END OF DEFINES------------------------------------------- */
/* ---------------------------------------------------------------------------------------- */

/* ---------------------------------------------------------------------------------------- */
/* -------------------------------START OF ENUMS DEFINITIONS------------------------------- */
/* ---------------------------------------------------------------------------------------- */

/* USER CODE BEGIN Registers */
/* Registers */
// All of Reg1 - RegN should be replaced by meaningful name of reg (don't delete prefixs)
typedef enum
{
	Read_BatterPackVoltage_ID = 0x01u,
	Read_Battery1to3Voltage_ID = 0x02u,
	Read_Battery4to6Voltage_ID = 0x03u,
	Read_MeasuredCurrent_ID = 0x04u,
	Read_MeasuredTemperature1to3_ID = 0x05u,
	Read_MeasuredTemperature4to6_ID = 0x06u,
	Read_MeasuredTemperature7to9_ID = 0x07u,
	Read_MeasuredTemperature10to12_ID = 0x08u,
	Read_MeasuredTemperature13to15_ID = 0x09u,
	Read_AmbientTemperature_ID = 0x0A
} ReadRegsID;

typedef enum
{
	Write_BatteryPackState_ID = 0x0Bu
} WriteRegsID;

typedef enum
{
	Error_Overvoltage_ID = 0x01u,
	Error_Undervoltage_ID = 0x02u,
	Error_Overcurrent_ID = 0x03u,
	Error_Overtemperature_ID = 0x04u
} ErrorRegsID;
/* USER CODE END Registers */

/* Enums to avoid magic numbers */
typedef enum
{
	ReadMessage,
	ReadRegID
} ReadFrameID;

typedef enum
{
	ResponseRegID,
	ResponseData1,
	ResponseData2,
	ResponseData3,
	ResponseData4,
	ResponseData5,
	ResponseData6
} ResponseFrame;

typedef enum
{
	WriteMessage_reg,
	WriteState
} WriteFrame;

typedef enum
{
	ErrorMessage_reg,
	ErrorRegID
} ErrorFrame;

typedef enum
{
	AcknowledgmentMessage_reg,
	WriteRegID
} AcknowledgmentFrame;

typedef enum
{
	Error_ReportMessage = 0x1Du,
	Read_RequestMessage = 0x3Du,
	Write_AcknowledgmentMessage = 0x5Du
} CANStadardMessage;

/* USER CODE BEGIN NodeAddress */
typedef enum
{
	Rx_ID = 0x003Au,
	Tx_ID = 0x003Fu
} NodeAddress;
/* USER CODE END NodeAddress */

enum
{
	BATTERY_PACK_STATE = 0U,
	BATTERY_PACK_VOLTAGE = 0U,
	BATTERY_PACK_V1to3 = 1U,
	BATTERY_PACK_V1 = 1U,
	BATTERY_PACK_V2 = 1U,
	BATTERY_PACK_V3 = 1U,
	BATTERY_PACK_V4to6 = 2U,
	BATTERY_PACK_V4 = 2U,
	BATTERY_PACK_V5 = 2U,
	BATTERY_PACK_V6 = 2U,
	MEASURED_CURRENT = 3U,
	MEASURED_TEMPERATURE_1to3 = 4U,
	TEMP_1 = 4U,
	TEMP_2 = 4U,
	TEMP_3 = 4U,
	MEASURED_TEMPERATURE_4to6 = 5U,
	TEMP_4 = 5U,
	TEMP_5 = 5U,
	TEMP_6 = 5U,
	MEASURED_TEMPERATURE_7to9 = 6U,
	TEMP_7 = 6U,
	TEMP_8 = 6U,
	TEMP_9 = 6U,
	MEASURED_TEMPERATURE_10to12 = 7U,
	TEMP_10 = 7U,
	TEMP_11 = 7U,
	TEMP_12 = 7U,
	MEASURED_TEMPERATURE_13to15 = 8U,
	TEMP_13 = 8U,
	TEMP_14 = 8U,
	TEMP_15 = 8U,
	AMBIENT_TEMPERATURE = 9U,
};

enum {
	B1 = 0U,
	B2,
	B3,
	B4,
	B5,
	B6
};

enum {
	T1 = 0U,
	T2,
	T3,
	T4,
	T5,
	T6,
	T7,
	T8,
	T9,
	T10,
	T11,
	T12,
	T13,
	T14,
	T15,
	T_AMBIENT
};

/* ---------------------------------------------------------------------------------------- */
/* -------------------------------END OF ENUMS DEFINITIONS--------------------------------- */
/* ---------------------------------------------------------------------------------------- */

/* ---------------------------------------------------------------------------------------- */
/* ---------------------------START OF VARIABLES DEFINITIONS------------------------------ */
/* ---------------------------------------------------------------------------------------- */

typedef void (*ReadReactionHandlerFuncPtr)(void);
typedef void (*WriteReactionHandlerFuncPtr)(void);

typedef struct
{
	WriteRegsID Write_RegID;
	WriteReactionHandlerFuncPtr Write_ReactionHandler;
	bool *Write_State;
} WriteMessageFrame;

typedef struct
{
	uint8_t Response_DLC;
	ReadRegsID Response_RegID;
	ReadReactionHandlerFuncPtr Read_ReactionHandler;
	uint8_t *Response_Data1;
	uint8_t *Response_Data2;
	uint8_t *Response_Data3;
	uint8_t *Response_Data4;
	uint8_t *Response_Data5;
	uint8_t *Response_Data6;
} ResponseMessageFrame;

/* ---------------------------------------------------------------------------------------- */
/* ---------------------------END OF STRUCTURES DEFINITIONS-------------------------------- */
/* ---------------------------------------------------------------------------------------- */

/* ---------------------------------------------------------------------------------------- */
/* ---------------------------START OF FUNCTIONS DECLARATIONS------------------------------ */
/* ---------------------------------------------------------------------------------------- */

void CAN_Init(void);
void CAN_On_Receive(uint8_t *RxData);
void CAN_Receive(CAN_HandleTypeDef *CANPointer, CAN_RxHeaderTypeDef *RxHeader, uint8_t *RxData);
void CAN_Transmit(CAN_TxHeaderTypeDef *TxHeader, uint8_t TxDLC, uint8_t *TxData, uint32_t *TxMailbox);
void CAN_Respond(void);
void CAN_ProcessWriteCommand(void);
void CAN_AcknowledgeWriteMessage(WriteRegsID WriteReqID);
void CAN_ReportError(ErrorRegsID ErrorID);
void CANBUS_Error_Handler(void);
void Read_BatterPackVoltage_Handler(void);
void Read_Battery1to3Voltage_Handler(void);
void Read_Battery4to6Voltage_Handler(void);
void Read_MeasuredCurrent_Handler(void);
void Read_MeasuredTemperature1to3_Handler(void);
void Read_MeasuredTemperature4to6_Handler(void);
void Read_MeasuredTemperature7to9_Handler(void);
void Read_MeasuredTemperature10to12_Handler(void);
void Read_MeasuredTemperature13to15_Handler(void);
void Read_AmbientTemperature_Handler(void);
void Write_BatteryPackState_Handler(void);

/* ---------------------------------------------------------------------------------------- */
/* ---------------------------END OF FUNCTIONS DECLARATIONS-------------------------------- */
/* ---------------------------------------------------------------------------------------- */

#endif /* INC_CAN_CONFIG_H_ */
