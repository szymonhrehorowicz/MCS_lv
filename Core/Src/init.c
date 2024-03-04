#include "init.h"
#include "errors.h"

void multiplexersInit(MultiplexerSetup *voltageMultiplexer, MultiplexerSetup *temperatureMultiplexer) {

	voltageMultiplexer->type = VOLTAGE_MULTIPLEXER_TYPE;
	voltageMultiplexer->portA = uC_A_GPIO_Port;
	voltageMultiplexer->pinA = uC_A_Pin;
	voltageMultiplexer->maskA = UC_A_MASK;
	voltageMultiplexer->portB = uC_B_GPIO_Port;
	voltageMultiplexer->pinB = uC_B_Pin;
	voltageMultiplexer->maskB = UC_B_MASK;
	voltageMultiplexer->portC = uC_C_GPIO_Port;
	voltageMultiplexer->pinC = uC_C_Pin;
	voltageMultiplexer->maskC = UC_C_MASK;

	temperatureMultiplexer->type = TEMPERATURE_MULTIPLEXER_TYPE;
	temperatureMultiplexer->portA = uC_S0_GPIO_Port;
	temperatureMultiplexer->pinA = uC_S0_Pin;
	temperatureMultiplexer->maskA = UC_S0_MASK;
	temperatureMultiplexer->portB = uC_S1_GPIO_Port;
	temperatureMultiplexer->pinB = uC_S1_Pin;
	temperatureMultiplexer->maskB = UC_S1_MASK;
	temperatureMultiplexer->portC = uC_S2_GPIO_Port;
	temperatureMultiplexer->pinC = uC_S2_Pin;
	temperatureMultiplexer->maskC = UC_S2_MASK;
	temperatureMultiplexer->portD = uC_S3_GPIO_Port;
	temperatureMultiplexer->pinD = uC_S3_Pin;
	temperatureMultiplexer->maskD = UC_S3_MASK;
}

void measureEntitiesInit(MeasureSetup *voltageMeasure, MeasureSetup *temperatureMeasure, SetOfEntities *voltages, SetOfEntities *temperatures) {
	voltageMeasure->startPort = uC_INH_GPIO_Port;
	voltageMeasure->startPin = uC_INH_Pin;
	voltageMeasure->firstCellToMeasure = BAT_1m;
	voltageMeasure->lastCellToMeasure = BAT_6m;
	voltageMeasure->ADC_Channel = VOLTAGE_CHANNEL;
	voltageMeasure->maxValue = VOLTAGE_MAX_VALUE;
	voltageMeasure->minValue = VOLTAGE_MIN_VALUE;
	voltageMeasure->entities = voltages;
	voltageMeasure->overBoundaryError = OvervoltageError_Handler;
	voltageMeasure->underBoundaryError = UndervoltageError_Handler;

	temperatureMeasure->startPort = uC_E_GPIO_Port;
	temperatureMeasure->startPin = uC_E_Pin;
	temperatureMeasure->firstCellToMeasure = TEMP_1m;
	temperatureMeasure->lastCellToMeasure = TEMP_16m;
	temperatureMeasure->ADC_Channel = TEMPEARATURE_CHANNEL;
	temperatureMeasure->maxValue = TEMPERATURE_MAX_VALUE;
	temperatureMeasure->minValue = TEMPERATURE_MIN_VALUE;
	temperatureMeasure->entities = temperatures;
	temperatureMeasure->overBoundaryError = OvertemperatureErro_Handler;
	temperatureMeasure->underBoundaryError = NULL;
}
