/**
  ******************************************************************************
  * @file    Serial.h
  * @author  Chu Xuan Quang
  * @date    15-05-2022
  * @brief   uart2 program body
  ******************************************************************************
  * @attention UART2: PA2(TX) AND PA3(RX)
  ******************************************************************************
*/
#ifndef UART2_H
#define UART2_H
#include <stdint.h>
#include "stm32f10x.h"

void UART2_Begin (uint32_t baud);
void UART2_SendChar(char data);
void UART2_SendString(char *Str);

#endif
