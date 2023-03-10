#ifndef DELAY_TIMER_H
#define DELAY_TIMER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Đối với SMT32 có timer cơ bản có 4 timer: timer1 -> timer4 */
#define DELAY_TIMER             TIM4
/* Clock cấp vào timer cho timer 4 */
#define DELAY_CLK_ENABLE        RCC_APB1Periph_TIM4
/* Chọn xử lý ngắt của timer */
#define DELAY_IRQn              TIM4_IRQn
/* Tên hàm xử lý ngắt*/
#define DELAY_IRQHandler        TIM4_IRQHandler
/* Tần số clock của hệ thống, đối với stm32f103 thì max là 72Mhz */
#define APB_CLOCK_TIMER         72
/* Giá trị tràn của bộ timer: max nhất là 16 bit->  2 mũ 16 -1: 65535 */
#define PERIOD                  65000

void DelayTimer_Init(void);

/*1s: 1000MS*/
void DelayTimer_Ms(uint32_t ms);
/*1s: 1000 000 us*/
void DelayTimer_Us(uint32_t us);
/* Trả về đơn vị thời gian MS */
uint32_t Timer_GetTickMs(void);
/* Trả về đơn vị thời gian US */
uint32_t Timer_GetTickUs(void);

#endif

