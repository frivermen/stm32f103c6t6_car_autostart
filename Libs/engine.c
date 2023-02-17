#include "engine.h"
#include "usart.h"
#include "adc.h"
#include "relay.h"
#include <stdio.h>
// === CONFIG ===
#define MAX_ADC 4095 // 2 ^ 12 - 1 = 4095
#define MAX_VOLTAGE 173 // 17.3V
#define STARTED_VOLTAGE 135 //13.5V
#define PUMP_DELAY 3000 // 3 seconds
#define START_TIMER 4000 //4 seconds
// ==============

uint8_t startEngine() {
	uint32_t start_timer = 0;
	if (!engineStarted()) {
		ignition_on();
		HAL_Delay(PUMP_DELAY);
		start_timer = HAL_GetTick();
		
		while (read_voltage() < 110 && HAL_GetTick() - start_timer < START_TIMER) {
			starter_on();
		}
		starter_off();

		HAL_Delay(1000);
		if (engineStarted()) {
			return 1;
		}
		else {
			ignition_off();
			return 0;
		}
	}
	else {
		return 1;
	}
}

uint8_t stopEngine() {
	if (engineStarted()) {
		ignition_off();
		HAL_Delay(1000);

		if (!engineStarted()) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else {
		return 1;
	}
}

uint8_t read_voltage() {
	  return HAL_ADC_GetValue(&hadc1) * MAX_VOLTAGE / MAX_ADC;
}

uint8_t engineStarted() {
	return (read_voltage() > STARTED_VOLTAGE) ? 1 : 0;
}
