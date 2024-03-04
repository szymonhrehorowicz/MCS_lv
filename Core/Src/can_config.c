/*
 * can_config.c
 *
 *   Created on: 27.02.2022
 *  Modified on: 30.07.2022
 *       Author: Krystian Sosin
 *      Version: 1.0.2
 *  Last change: Fix minor bugs and add CAN_AcknowledgeWriteMessage() function.
 */

#include "main.h"
#include "can_config.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_can.h"
#include "stdbool.h"

/* Variables and arrays needed to proper work of CAN */
CAN_FilterTypeDef sFilterConfig;
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
uint8_t TxData[8];
uint8_t RxData[8];
uint32_t TxMailbox;

/* USER CODE BEGIN Externs */
/* Externs used in configs */
extern bool batteryPackState;

extern SetOfEntities voltages;
extern SetOfEntities temperatures;

extern uint8_t Read_MeasuredCurrent[2];

/* USER CODE END Externs */

/**
 * CAN READ MESSAGE FRAME
 *
 * | RxID  |  2  | 0x3D | ADDR  |
 * | StdID | DLC | READ | RegID |
 *
 *
 */
// Enum of ReadRegs defined in can_config.h. Nothing needs to be configured here.

/* USER CODE BEGIN ResponseMessage */

/**
 *  CAN RESPONSE MESSAGE FRAME
 *
 * | TxID  | DLC | ADDR  | VALUE  | ... | VALUE  |
 * | StdID | DLC | RegID | DATA_1 | ... | DATA_N |
 *
 **/

ResponseMessageFrame ResponseMessage[NUMBER_OF_READ_REGS] =
	{
		{
			.Response_DLC = 3u,
			.Read_ReactionHandler = Read_BatterPackVoltage_Handler,
			.Response_RegID = Read_BatterPackVoltage_ID,
			.Response_Data1 = &voltages.overallPercentageValue[MOST_SIGNIFICANT_BYTE],
			.Response_Data2 = &voltages.overallPercentageValue[LESS_SIGNIFICANT_BYTE],
		},
		{
			.Response_DLC = 7u,
			.Read_ReactionHandler = Read_Battery1to3Voltage_Handler,
			.Response_RegID = Read_Battery1to3Voltage_ID,
			.Response_Data1 = &voltages.values[B1].value[MOST_SIGNIFICANT_BYTE],
			.Response_Data2 = &voltages.values[B1].value[LESS_SIGNIFICANT_BYTE],
			.Response_Data3 = &voltages.values[B2].value[MOST_SIGNIFICANT_BYTE],
			.Response_Data4 = &voltages.values[B2].value[LESS_SIGNIFICANT_BYTE],
			.Response_Data5 = &voltages.values[B3].value[MOST_SIGNIFICANT_BYTE],
			.Response_Data6 = &voltages.values[B2].value[LESS_SIGNIFICANT_BYTE],
		},
		{
			.Response_DLC = 7u,
			.Read_ReactionHandler = Read_Battery4to6Voltage_Handler,
			.Response_RegID = Read_Battery4to6Voltage_ID,
			.Response_Data1 = &voltages.values[B4].value[MOST_SIGNIFICANT_BYTE],
			.Response_Data2 = &voltages.values[B4].value[LESS_SIGNIFICANT_BYTE],
			.Response_Data3 = &voltages.values[B5].value[MOST_SIGNIFICANT_BYTE],
			.Response_Data4 = &voltages.values[B5].value[LESS_SIGNIFICANT_BYTE],
			.Response_Data5 = &voltages.values[B6].value[MOST_SIGNIFICANT_BYTE],
			.Response_Data6 = &voltages.values[B6].value[LESS_SIGNIFICANT_BYTE],
		},
		{
			.Response_DLC = 3u,
			.Read_ReactionHandler = Read_MeasuredCurrent_Handler,
			.Response_RegID = Read_MeasuredCurrent_ID,
			.Response_Data1 = &Read_MeasuredCurrent[MOST_SIGNIFICANT_BYTE],
			.Response_Data2 = &Read_MeasuredCurrent[LESS_SIGNIFICANT_BYTE],
		},
		{
			.Response_DLC = 7u,
			.Read_ReactionHandler = Read_MeasuredTemperature1to3_Handler,
			.Response_RegID = Read_MeasuredTemperature1to3_ID,
			.Response_Data1 = &temperatures.values[T1].value[MOST_SIGNIFICANT_BYTE],
			.Response_Data1 = &temperatures.values[T1].value[LESS_SIGNIFICANT_BYTE],
			.Response_Data3 = &temperatures.values[T2].value[MOST_SIGNIFICANT_BYTE],
			.Response_Data4 = &temperatures.values[T2].value[LESS_SIGNIFICANT_BYTE],
			.Response_Data5 = &temperatures.values[T3].value[MOST_SIGNIFICANT_BYTE],
			.Response_Data6 = &temperatures.values[T3].value[LESS_SIGNIFICANT_BYTE],
		},
		{
			.Response_DLC = 7u,
			.Read_ReactionHandler = Read_MeasuredTemperature4to6_Handler,
			.Response_RegID = Read_MeasuredTemperature4to6_ID,
			.Response_Data1 = &temperatures.values[T4].value[MOST_SIGNIFICANT_BYTE],
			.Response_Data2 = &temperatures.values[T4].value[LESS_SIGNIFICANT_BYTE],
			.Response_Data3 = &temperatures.values[T5].value[MOST_SIGNIFICANT_BYTE],
			.Response_Data4 = &temperatures.values[T5].value[LESS_SIGNIFICANT_BYTE],
			.Response_Data5 = &temperatures.values[T6].value[MOST_SIGNIFICANT_BYTE],
			.Response_Data6 = &temperatures.values[T6].value[LESS_SIGNIFICANT_BYTE],
		},
		{
			.Response_DLC = 7u,
			.Read_ReactionHandler = Read_MeasuredTemperature7to9_Handler,
			.Response_RegID = Read_MeasuredTemperature7to9_ID,
			.Response_Data1 = &temperatures.values[T7].value[MOST_SIGNIFICANT_BYTE],
			.Response_Data2 = &temperatures.values[T7].value[LESS_SIGNIFICANT_BYTE],
			.Response_Data3 = &temperatures.values[T8].value[MOST_SIGNIFICANT_BYTE],
			.Response_Data4 = &temperatures.values[T8].value[LESS_SIGNIFICANT_BYTE],
			.Response_Data5 = &temperatures.values[T9].value[MOST_SIGNIFICANT_BYTE],
			.Response_Data6 = &temperatures.values[T9].value[LESS_SIGNIFICANT_BYTE],
		},
		{
			.Response_DLC = 7u,
			.Read_ReactionHandler = Read_MeasuredTemperature10to12_Handler,
			.Response_RegID = Read_MeasuredTemperature10to12_ID,
			.Response_Data1 = &temperatures.values[T10].value[MOST_SIGNIFICANT_BYTE],
			.Response_Data2 = &temperatures.values[T10].value[LESS_SIGNIFICANT_BYTE],
			.Response_Data3 = &temperatures.values[T11].value[MOST_SIGNIFICANT_BYTE],
			.Response_Data4 = &temperatures.values[T11].value[LESS_SIGNIFICANT_BYTE],
			.Response_Data5 = &temperatures.values[T12].value[MOST_SIGNIFICANT_BYTE],
			.Response_Data6 = &temperatures.values[T12].value[LESS_SIGNIFICANT_BYTE],
		},
		{
			.Response_DLC = 7u,
			.Read_ReactionHandler = Read_MeasuredTemperature13to15_Handler,
			.Response_RegID = Read_MeasuredTemperature13to15_ID,
			.Response_Data1 = &temperatures.values[T13].value[MOST_SIGNIFICANT_BYTE],
			.Response_Data2 = &temperatures.values[T13].value[LESS_SIGNIFICANT_BYTE],
			.Response_Data3 = &temperatures.values[T14].value[MOST_SIGNIFICANT_BYTE],
			.Response_Data4 = &temperatures.values[T14].value[LESS_SIGNIFICANT_BYTE],
			.Response_Data5 = &temperatures.values[T15].value[MOST_SIGNIFICANT_BYTE],
			.Response_Data6 = &temperatures.values[T15].value[LESS_SIGNIFICANT_BYTE],
		},
		{
			.Response_DLC = 3u,
			.Read_ReactionHandler = Read_AmbientTemperature_Handler,
			.Response_RegID = Read_AmbientTemperature_ID,
			.Response_Data1 = &temperatures.values[T_AMBIENT].value[MOST_SIGNIFICANT_BYTE],
			.Response_Data2 = &temperatures.values[T_AMBIENT].value[LESS_SIGNIFICANT_BYTE],
		},
};
/* USER CODE END ResponseMessage */

/* USER CODE BEGIN WriteMessage */

/**
 *  CAN WRITE MESSAGE FRAME
 *
 * | RxID  | DLC | ADDR  | VALUE  | ... | VALUE  |
 * | StdID | DLC | WRITE | DATA_1 | ... | DATA_N |
 *
 **/
WriteMessageFrame WriteMessage[NUMBER_OF_WRITE_REGS] =
	{
		{
			.Write_RegID = Write_BatteryPackState_ID,
			.Write_ReactionHandler = Write_BatteryPackState_Handler,
			.Write_State = &batteryPackState,
		},
};
/* USER CODE END WriteMessage */
/**
 *  CAN ERROR MESSAGE FRAME
 *
 * | TxID  |  2  | 0x1D  |    ID   |
 * | StdID | DLC | ERROR | ErrorID |
 *
 */
// Enum of ErrorRegs defined in can_config.h. Nothing needs to be configured here.

/** CAN_Init
 * @brief Function to ensure proper work of CAN interface
		  - configuration of filter and calling essantial functions of CAN initialization
		  Filter configured in accordance with E&S Team Project Guidlines.
 *
 * @retval None.
 **/
void CAN_Init(void)
{
	sFilterConfig.FilterBank = 1;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = Rx_ID << 5;
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0xFFFF << 5;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.SlaveStartFilterBank = 14;

	if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
	{
		/* Filter configuration Error */
		Error_Handler();
	}
	if (HAL_CAN_Start(&hcan) != HAL_OK)
	{
		/* Start Error */
		Error_Handler();
	}
	if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK)
	{
		/* Notification Error */
		Error_Handler();
	}

	TxHeader.StdId = Tx_ID;
	TxHeader.ExtId = 0x0000;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = DISABLE;
}

/** HAL_CAN_RxFifo0MsgPendingCallback
 * @brief HAL Callback to handle interuption from CAN new message
 *
 * @param hcan pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 *
 * @retval None
 **/
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_Receive(hcan, &RxHeader, RxData);
	CAN_On_Receive(RxData);
}

/** CAN_On_Receive
 * @brief Function to procces received message - checking if message is read or write command and call specific handler
 *
 * @param RxData pointer to uint8_t array which stores received data
 *
 * @retval None
 **/
void CAN_On_Receive(uint8_t *RxData)
{
	if (Read_RequestMessage == RxData[ReadMessage])
	{
		CAN_Respond();
	}
	else
	{
		CAN_ProcessWriteCommand();
	}
}

/** CAN_Receive
 * @brief Function to receive data via CAN and, if neccessary, report error of CAN connection
 *
 * @param CANPointerpointer to a CAN_HandleTypeDef structure that contains
 *                          the configuration information for the specified CAN.
 * @param RxHeader CAN_RxHeaderTypeDef pointer to structure that contains RxHeader configuration.
 * @param RxData uint8_t pointer to array that will contain received data.
 *
 * @retval None.
 **/
void CAN_Receive(CAN_HandleTypeDef *CANPointer, CAN_RxHeaderTypeDef *RxHeader, uint8_t *RxData)
{
	if (HAL_CAN_GetRxMessage(CANPointer, CAN_RX_FIFO0, RxHeader, RxData) != HAL_OK)
	{
		CANBUS_Error_Handler();
	}
};

/** CAN_Transmit
 * @brief Function to transmit data via CAN and, if neccessary, report error of CAN connection
 *
 * @param TxHeader CAN_TxHeaderTypeDef pointer to structure that contains TxHeader configuration.
 * @param TxDLC uint8_t variable which contains Date Length of CAN message.
 * @param TxData uint8_t pointer to array that contains data to transmit.
 * @param TxMailbox uint32_t pointer to array that contains whole CAN message to transmit.
 *
 * @retval None.
 **/
void CAN_Transmit(CAN_TxHeaderTypeDef *TxHeader, uint8_t TxDLC, uint8_t *TxData, uint32_t *TxMailbox)
{
	TxHeader->DLC = TxDLC;
	if (HAL_CAN_AddTxMessage(&hcan, TxHeader, TxData, TxMailbox) != HAL_OK)
	{
		CANBUS_Error_Handler();
	}
}

/** CAN_Respond
 * @brief Function to respond in connection with read request from MCU
 *
 * @retval None.
 **/
void CAN_Respond(void)
{
	for (int i = FIRST_ARRAY_ELEMENT; i < NUMBER_OF_READ_REGS; i++)
	{
		if (ResponseMessage[i].Response_RegID == RxData[ReadRegID])
		{
			ResponseMessage[i].Read_ReactionHandler();
		}
	}
}

/** CAN_ProcessWriteCommand
 * @brief Function to process write command
 *
 * @retval None.
 **/
void CAN_ProcessWriteCommand(void)
{
	for (int i = FIRST_ARRAY_ELEMENT; i < NUMBER_OF_WRITE_REGS; i++)
	{
		if (WriteMessage[i].Write_RegID == RxData[WriteMessage_reg])
		{
			CAN_AcknowledgeWriteMessage(WriteMessage[i].Write_RegID);
			WriteMessage[i].Write_ReactionHandler();
		}
	}
}

/** CAN_AcknowledgeWriteMessage
 * @brief Function to send acknowledment received write instruction via CAN
 *
 * @param WriteReqID ID of received write instruction
 *
 * @retval None.
 **/
void CAN_AcknowledgeWriteMessage(WriteRegsID WriteReqID)
{
	TxData[AcknowledgmentMessage_reg] = Write_AcknowledgmentMessage; // 1st Data Byte: Standard Write Acknowledgment instruction
	TxData[WriteRegID] = WriteReqID;								 // 2nd Data Byte: Acknowledged Received Write Command ReqID
	CAN_Transmit(&TxHeader, ACKNOWLEDMENT_DLC, TxData, &TxMailbox);	 // Transmit Data
}

/** CAN_ReportError
 * @brief Function to report error via CAN
 *
 * @param ErrorID ID of reported Error Register
 *
 * @retval None.
 **/
void CAN_ReportError(ErrorRegsID ErrorID)
{
	TxData[ErrorMessage_reg] = Error_ReportMessage;			// 1st Data Byte: Standard Error Report instruction
	TxData[ErrorRegID] = ErrorID;							// 2nd Data Byte: Reported Error ID
	CAN_Transmit(&TxHeader, ERROR_DLC, TxData, &TxMailbox); // Transmit Data
}

/* USER CODE BEGIN CANBUS_Error_Handler */

/** CANBUS_Error_Handler
 * @brief General error handler of CAN connection and communication
 *
 * @retval None.
 * */
void CANBUS_Error_Handler(void)
{
	__disable_irq();
	/*
	Put here behaviour of ECU when error will be occured.
	*/
}

/* USER CODE BEGIN ReadReactionHandlers */

void Read_BatterPackVoltage_Handler(void)
{
	TxData[ResponseRegID] = ResponseMessage[BATTERY_PACK_VOLTAGE].Response_RegID;
	TxData[ResponseData1] = *(ResponseMessage[BATTERY_PACK_VOLTAGE].Response_Data1);
	TxData[ResponseData2] = *(ResponseMessage[BATTERY_PACK_VOLTAGE].Response_Data2);

	CAN_Transmit(&TxHeader, ResponseMessage[BATTERY_PACK_VOLTAGE].Response_DLC, TxData, &TxMailbox);
}

void Read_Battery1to3Voltage_Handler(void)
{
	TxData[ResponseRegID] = ResponseMessage[BATTERY_PACK_V1to3].Response_RegID;
	TxData[ResponseData1] = *(ResponseMessage[BATTERY_PACK_V1].Response_Data1);
	TxData[ResponseData2] = *(ResponseMessage[BATTERY_PACK_V1].Response_Data2);
	TxData[ResponseData3] = *(ResponseMessage[BATTERY_PACK_V2].Response_Data3);
	TxData[ResponseData4] = *(ResponseMessage[BATTERY_PACK_V2].Response_Data4);
	TxData[ResponseData5] = *(ResponseMessage[BATTERY_PACK_V3].Response_Data5);
	TxData[ResponseData6] = *(ResponseMessage[BATTERY_PACK_V3].Response_Data6);

	CAN_Transmit(&TxHeader, ResponseMessage[BATTERY_PACK_V1to3].Response_DLC, TxData, &TxMailbox);
}

void Read_Battery4to6Voltage_Handler(void)
{
	TxData[ResponseRegID] = ResponseMessage[BATTERY_PACK_V4to6].Response_RegID;
	TxData[ResponseData1] = *(ResponseMessage[BATTERY_PACK_V4].Response_Data1);
	TxData[ResponseData2] = *(ResponseMessage[BATTERY_PACK_V4].Response_Data2);
	TxData[ResponseData3] = *(ResponseMessage[BATTERY_PACK_V5].Response_Data3);
	TxData[ResponseData4] = *(ResponseMessage[BATTERY_PACK_V5].Response_Data4);
	TxData[ResponseData5] = *(ResponseMessage[BATTERY_PACK_V6].Response_Data5);
	TxData[ResponseData6] = *(ResponseMessage[BATTERY_PACK_V6].Response_Data6);

	CAN_Transmit(&TxHeader, ResponseMessage[BATTERY_PACK_V4to6].Response_DLC, TxData, &TxMailbox);
}

void Read_MeasuredCurrent_Handler(void)
{
	TxData[ResponseRegID] = ResponseMessage[MEASURED_CURRENT].Response_RegID;
	TxData[ResponseData1] = *(ResponseMessage[MEASURED_CURRENT].Response_Data1);
	TxData[ResponseData2] = *(ResponseMessage[MEASURED_CURRENT].Response_Data2);

	CAN_Transmit(&TxHeader, ResponseMessage[MEASURED_CURRENT].Response_DLC, TxData, &TxMailbox);
}

void Read_MeasuredTemperature1to3_Handler(void)
{
	TxData[ResponseRegID] = ResponseMessage[MEASURED_TEMPERATURE_1to3].Response_RegID;
	TxData[ResponseData1] = *(ResponseMessage[TEMP_1].Response_Data1);
	TxData[ResponseData2] = *(ResponseMessage[TEMP_1].Response_Data2);
	TxData[ResponseData3] = *(ResponseMessage[TEMP_2].Response_Data3);
	TxData[ResponseData4] = *(ResponseMessage[TEMP_2].Response_Data4);
	TxData[ResponseData5] = *(ResponseMessage[TEMP_3].Response_Data5);
	TxData[ResponseData6] = *(ResponseMessage[TEMP_3].Response_Data6);

	CAN_Transmit(&TxHeader, ResponseMessage[MEASURED_TEMPERATURE_1to3].Response_DLC, TxData, &TxMailbox);
}

void Read_MeasuredTemperature4to6_Handler(void)
{
	TxData[ResponseRegID] = ResponseMessage[MEASURED_TEMPERATURE_4to6].Response_RegID;
	TxData[ResponseData1] = *(ResponseMessage[TEMP_4].Response_Data1);
	TxData[ResponseData2] = *(ResponseMessage[TEMP_4].Response_Data2);
	TxData[ResponseData3] = *(ResponseMessage[TEMP_5].Response_Data3);
	TxData[ResponseData4] = *(ResponseMessage[TEMP_5].Response_Data4);
	TxData[ResponseData5] = *(ResponseMessage[TEMP_6].Response_Data5);
	TxData[ResponseData6] = *(ResponseMessage[TEMP_6].Response_Data6);

	CAN_Transmit(&TxHeader, ResponseMessage[MEASURED_TEMPERATURE_4to6].Response_DLC, TxData, &TxMailbox);
}

void Read_MeasuredTemperature7to9_Handler(void)
{
	TxData[ResponseRegID] = ResponseMessage[MEASURED_TEMPERATURE_7to9].Response_RegID;
	TxData[ResponseData1] = *(ResponseMessage[TEMP_7].Response_Data1);
	TxData[ResponseData2] = *(ResponseMessage[TEMP_7].Response_Data2);
	TxData[ResponseData3] = *(ResponseMessage[TEMP_8].Response_Data3);
	TxData[ResponseData4] = *(ResponseMessage[TEMP_8].Response_Data4);
	TxData[ResponseData5] = *(ResponseMessage[TEMP_9].Response_Data5);
	TxData[ResponseData6] = *(ResponseMessage[TEMP_9].Response_Data6);

	CAN_Transmit(&TxHeader, ResponseMessage[MEASURED_TEMPERATURE_7to9].Response_DLC, TxData, &TxMailbox);
}

void Read_MeasuredTemperature10to12_Handler(void)
{
	TxData[ResponseRegID] = ResponseMessage[MEASURED_TEMPERATURE_10to12].Response_RegID;
	TxData[ResponseData1] = *(ResponseMessage[TEMP_10].Response_Data1);
	TxData[ResponseData2] = *(ResponseMessage[TEMP_10].Response_Data2);
	TxData[ResponseData3] = *(ResponseMessage[TEMP_11].Response_Data3);
	TxData[ResponseData4] = *(ResponseMessage[TEMP_11].Response_Data4);
	TxData[ResponseData5] = *(ResponseMessage[TEMP_12].Response_Data5);
	TxData[ResponseData6] = *(ResponseMessage[TEMP_12].Response_Data6);

	CAN_Transmit(&TxHeader, ResponseMessage[MEASURED_TEMPERATURE_10to12].Response_DLC, TxData, &TxMailbox);
}

void Read_MeasuredTemperature13to15_Handler(void)
{
	TxData[ResponseRegID] = ResponseMessage[MEASURED_TEMPERATURE_13to15].Response_RegID;
	TxData[ResponseData1] = *(ResponseMessage[TEMP_13].Response_Data1);
	TxData[ResponseData2] = *(ResponseMessage[TEMP_13].Response_Data2);
	TxData[ResponseData3] = *(ResponseMessage[TEMP_14].Response_Data3);
	TxData[ResponseData4] = *(ResponseMessage[TEMP_14].Response_Data4);
	TxData[ResponseData5] = *(ResponseMessage[TEMP_15].Response_Data5);
	TxData[ResponseData6] = *(ResponseMessage[TEMP_15].Response_Data6);

	CAN_Transmit(&TxHeader, ResponseMessage[MEASURED_TEMPERATURE_13to15].Response_DLC, TxData, &TxMailbox);
}

void Read_AmbientTemperature_Handler(void)
{
	TxData[ResponseRegID] = ResponseMessage[AMBIENT_TEMPERATURE].Response_RegID;
	TxData[ResponseData1] = *(ResponseMessage[AMBIENT_TEMPERATURE].Response_Data1);
	TxData[ResponseData2] = *(ResponseMessage[AMBIENT_TEMPERATURE].Response_Data2);
}

/* USER CODE END ReadReactionHandlers */

/* USER CODE BEGIN WriteReactionHandlers */

/** Write_BatteryPackState_Handler
 * @brief Function that turns off battery pack in case of an error.
 *
 * @param void
 *
 * @retval None.
 **/
void Write_BatteryPackState_Handler(void)
{
	*(WriteMessage[BATTERY_PACK_STATE].Write_State) = RxData[WriteState];
}

/* USER CODE END WriteReactionHandlers */
