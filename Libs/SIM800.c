#include "SIM800.h"
#include "usart.h"
#include "engine.h"
#include <string.h>
#include <stdio.h>

uint8_t rx_data = 0;
uint8_t rx_buffer[255] = {0};
uint8_t rx_index = 0;

void Sim800_RxCallBack(void) {
	rx_buffer[rx_index++] = rx_data;
	rx_buffer[rx_index] = '\0';
    HAL_UART_Receive_IT(UART_SIM800, &rx_data, 1);
}

uint8_t SIM800_SendCommand(char *command, char *reply, uint16_t delay) {
	uint8_t error = 0;
	rx_index = 0;
	rx_buffer[0] = '\0';

	HAL_UART_Transmit_IT(UART_SIM800, (uint8_t *) command, (uint16_t) strlen(command));

    while (delay > 0 && error == 0) {
    	if (strstr((char *)rx_buffer, reply)) {
    		error = 1;
    	}
    	HAL_Delay(100);
    	delay -= 100;
    }

    return error;
}

uint8_t SIM800_Init(void) {
    uint8_t error = 0;
    HAL_UART_Receive_IT(UART_SIM800, &rx_data, 1);
    error += SIM800_SendCommand("AT\r\n", "OK\r\n", CMD_DELAY);
    error += SIM800_SendCommand("ATE0\r\n", "OK\r\n", CMD_DELAY);
    error += SIM800_SendCommand("AT+CLIP=1\r\n", "OK\r\n", CMD_DELAY);
    error += SIM800_SendCommand("AT+GSMBUSY=1\r\n", "OK\r\n", CMD_DELAY);
    error += SIM800_SendCommand("AT+CMGF=1\r\n", "OK\r\n", CMD_DELAY);
    error += SIM800_SendCommand("AT+CMGDA=\"DEL ALL\"\r\n", "OK\r\n", CMD_DELAY);
    return error;
}

uint8_t SIM800_DelAllSMS() {
    return SIM800_SendCommand("AT+CMGDA=\"DEL ALL\"\r\n", "OK\r\n", CMD_DELAY);
}

uint8_t SIM800_ParseSMS(char sms_index) {
	char phone_number[20] = {0};
	char message[30] ={0};
	char buf[12] = {0};
	char *p;
	uint8_t res;

	sprintf(buf, "AT+CMGR=%c\r\n", sms_index);
	SIM800_SendCommand(buf, "+CMGR", CMD_DELAY);
	p = strstr((char *)rx_buffer, "CMGR");
	res = sscanf(p, "%*[^+]%[^\"]%*[^\n]\n%[^\r]", phone_number, message);
	if (res == 2) {
		if (strstr(phone_number, PHONE_STRING1)) {
			if (strstr(message, "start")) {
				return 1;
			}
			else if (strstr(message, "stop")) {
				return 2;
			}
		}
		else {
			if (strstr(message, "hello kitty start")) {
				return 1;
			}
			else if (strstr(message, "hello kitty stop")) {
				return 2;
			}
		}
	}
	return 0;
}
char SIM800_CheckSMS() {
	char *p;
	int available;
	SIM800_SendCommand("AT+CPMS?\r\n", "+CPMS:", CMD_DELAY);
	p = strstr((char *)rx_buffer, "+CPMS:");
	if (p) {
		sscanf(p, "%*[^,],%*d,%*d,%*[^,],%*d,%*d,%*[^,],%d,%*d", &available);
		if (available) {
			return '0' + available;
		}
	}
	return 0;
}

uint8_t SIM800_Call() {
	return SIM800_SendCommand(PHONE_STRING2, "OK\r\n", CMD_DELAY);
}

void SIM800_SendSMS(char *message) {
	char buf[20] = {0};

	sprintf(buf, "%s%c", message, 26);
	SIM800_SendCommand(PHONE_STRING3, ">", CMD_DELAY);
	SIM800_SendCommand(buf, "+CMGS", CMD_DELAY);
	return;
}
