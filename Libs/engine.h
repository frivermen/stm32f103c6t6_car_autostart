#ifndef __engine_H
#define __engine_H

#include <stdint.h>

uint8_t startEngine();

uint8_t stopEngine();

uint8_t read_voltage(void);

uint8_t engineStarted(void);

#endif // __engine_H
