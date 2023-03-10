#include "ADC.h"
#include "stm32f10x.h"
#include "DelayTimer.h"
#include "LCD_I2C.h"
#include "math.h"
#include "stdio.h"
void ADC_DMA_Configure(uint8_t ADC_NumberChannel, uint16_t *ADC_Value)
{
	/*clock sorce cho ADC duoi 16 MHz*/
	RCC -> CFGR   |= RCC_CFGR_ADCPRE_DIV6;
	/*Cap clocj cho ADC, IOPAA , AFIO*/
	RCC -> APB2ENR |= RCC_APB2ENR_ADC1EN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
	RCC -> AHBENR  |= RCC_AHBENR_DMA1EN;
	/*Cau hinh cho ADC channel 5*/
	GPIOA -> CRL   |= GPIO_CRL_CNF5_1;
	GPIOA -> CRL   &=~(GPIO_CRL_CNF5_0);
	GPIOA -> CRL   |= GPIO_CRL_CNF4_1;
	GPIOA -> CRL   &=~(GPIO_CRL_CNF4_0);
	/*Cau hinh toc do lay mau*/
	ADC1 -> SMPR2   |= 0x00038000;
	/*Cau hinh so kenh: 1 . Cau hinh ... cau hinh cho do uu tien doc trong ADC */
	ADC1 -> SQR1    |= 0x00100000;
	ADC1 -> SQR3    |= 0x80;
	ADC1 -> SQR3    |= 0x05;
	/*Vi ly do la doc > 2 channel do do Ma Bat bit ADC SCAN*/
	ADC1 -> CR1    |= ADC_CR1_SCAN;
	//SD DMA Bat bit DMA len
	ADC1 -> CR2    |= ADC_CR2_DMA;
	//Cau hinh cho DMA
	//Doc du lieu tu dau 
	DMA1_Channel1 -> CPAR     = (uint32_t)(&(ADC1 -> DR));
	DMA1_Channel1 -> CMAR     = (uint32_t)ADC_Value;
	DMA1_Channel1 -> CNDTR    = ADC_NumberChannel;
	DMA1_Channel1 -> CCR     |= DMA_CCR1_CIRC  | DMA_CCR1_MINC | DMA_CCR1_MSIZE_0 | DMA_CCR1_PSIZE_0;
	DMA1_Channel1 -> CCR     |= DMA_CCR1_EN;
	/*Enable cho ADC*/
	ADC1 -> CR2     |= ADC_CR2_ADON | ADC_CR2_CONT;
	DelayTimer_Ms(1);
	ADC1 -> CR2     |= ADC_CR2_ADON;
	DelayTimer_Ms(1);
	ADC1 -> CR2     |= ADC_CR2_CAL;
	DelayTimer_Ms(2);
}
//int Map(int x, int inMin, int inMax, int outMin, int outMax) {
//  return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
//}
void GPIO_Configure(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
//void PWM_Config(int goc){
//	/*Cau hinh cho TIME2*/
//	RCC -> APB1ENR |= 0x02;
//	/*Cau hinh cho bo 3 cua TIMER*/
//	TIM3 -> PSC     = 72 - 1;
//	TIM3 -> ARR     = 20000 - 1;
//	TIM3 -> CNT     = 0;
//	/*Cau hinh cho PWM 1 channel ( PA0 - Channel 1 cua TIMER2*/
//	TIM3 -> CCMR2   |= 0x0060;
//	TIM3 -> CCER    |= 0x0100;
//	int T;
//	T = Map(goc, 0, 180, 500, 2500);
//	TIM3 -> CCR3     = T;
//	/*Cho phep bo dem hoat dong*/
//	TIM3 -> CR1     |= 0x01;
//	TIM3 -> EGR     |= 0x01;
//}