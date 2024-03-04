#include "errors.h"

/*
 *
 * Functions below provide wrappers for errors that will be transmitted via CAN to the MCU_Main.
 *
 * */

void OvervoltageError_Handler(void) {
	CAN_ReportError(Error_Overvoltage_ID);
}

void UndervoltageError_Handler(void) {
	CAN_ReportError(Error_Undervoltage_ID);
}

void OvercurrentError_Handler(void) {
	CAN_ReportError(Error_Overcurrent_ID);
}

void OvertemperatureErro_Handler(void) {
	CAN_ReportError(Error_Overtemperature_ID);
}
