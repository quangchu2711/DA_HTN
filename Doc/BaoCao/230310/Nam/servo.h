#ifndef SERVO_H
#define SERVO_H
#include <stdint.h>
#include <stdbool.h>
#include "stm32f10x.h"

void SERVO_Init(void);

/* posValue: 0-180 */
bool SERVO_Rotage(uint8_t posValue);

#endif
