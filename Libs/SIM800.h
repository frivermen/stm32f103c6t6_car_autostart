#ifndef __SIM800_H
#define __SIM800_H

#include <stdint.h>


// === CONFIG ===
#define UART_SIM800 &huart2
#define CMD_DELAY   800
#define PHONE_STRING1 "+79121231212"
#define PHONE_STRING2 "ATD+79121231212;\r\n"
#define PHONE_STRING3 "AT+CMGS=\"+79121231212\" \r\n"
// ==============

void Sim800_RxCallBack(void);

uint8_t SIM800_SendCommand(char *command, char *reply, uint16_t delay);

uint8_t SIM800_Init(void);

uint8_t SIM800_DelAllSMS(void);

uint8_t SIM800_ParseSMS(char sms_index);

char SIM800_CheckSMS(void);

uint8_t SIM800_Call(void);

void SIM800_SendSMS(char *message);

#endif // __SIM800_H
