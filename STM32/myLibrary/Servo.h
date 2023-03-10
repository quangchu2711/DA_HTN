/**
  ******************************************************************************
  * @file    Servo.h
  * @author  Chu Xuan Quang
  * @date    24-1-2022
  * @brief   Servo program body
  ******************************************************************************
  * @attention on the stm32f103 kit only the following pins can be used:
	*						 PA0, PA1, PA2, PA3
	*						 PA6, PA7, PB0, PB1
	*						 PB6, PB7, PB8, PB9
  ******************************************************************************
*/
#ifndef SERVO_H
#define SERVO_H
#include "stm32f10x.h"
#include "PWM.h"

#define PSC 99
#define PERIOD 14399
#define value_for_duty_cycle_0_5_ms 360      //0.5 ms
#define value_for_duty_cycle_2_5_ms 1800     //2.5 ms

void Myservo_Attach(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_X);
void Myservo_Write(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_X, int16_t pos);
uint16_t Map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);

#endif
