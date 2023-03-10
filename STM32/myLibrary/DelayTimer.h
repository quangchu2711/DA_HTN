/**
  ******************************************************************************
  * @file    DelayTimer.h
  * @author  Chu Xuan Quang
  * @date    05-02-2022
  * @brief   DelayTimer program body
  ******************************************************************************
  * @attention TIMER2
  ******************************************************************************
*/
#ifndef DELAY_TIMER_H
#define DELAY_TIMER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

#define DELAY_TIMER					TIM2
#define DELAY_CLK_ENABLE		RCC_APB1Periph_TIM2
#define DELAY_IRQn 					TIM2_IRQn
#define DELAY_IRQHandler		TIM2_IRQHandler
#define APB_CLOCK_TIMER			72
#define PERIOD							65000

void DelayTimer_Init(void);
void DelayTimer_Ms(uint32_t ms);
void DelayTimer_Us(uint32_t us);
uint32_t Timer_GetTickMs(void);
uint32_t Timer_GetTickUs(void);

#endif

