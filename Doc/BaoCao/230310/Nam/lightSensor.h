#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H
#include <stdint.h>
#include <stdbool.h>
#include "stm32f10x.h"

void LIGHT_SENSOR_Init(void);

bool LIGHT_SENSOR_Read(uint8_t *pLightValue);

#endif
