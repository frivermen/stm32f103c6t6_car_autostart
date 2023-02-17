#ifndef _RELAY_H
#define RELAY1 GPIOB, 1 << 9
#define RELAY2 GPIOB, 1 << 8
#define RELAY3 GPIOB, 1 << 7
#define ignition_on() HAL_GPIO_WritePin(RELAY1, GPIO_PIN_SET);
#define ignition_off() HAL_GPIO_WritePin(RELAY1, GPIO_PIN_RESET);
#define starter_on() HAL_GPIO_WritePin(RELAY2, GPIO_PIN_SET);
#define starter_off() HAL_GPIO_WritePin(RELAY2, GPIO_PIN_RESET);
#define indicator_on() HAL_GPIO_WritePin(RELAY3, GPIO_PIN_SET);
#define indicator_off() HAL_GPIO_WritePin(RELAY3, GPIO_PIN_RESET);
#endif // _RELAY_H