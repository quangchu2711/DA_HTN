#include "stm32f10x.h"
void ADC_DMA_Configure(uint8_t ADC_NumberChannel, uint16_t *ADC_Value);
void ADC_Read_Gas();
void ADC_Read_CO();
void ADC_ADISPLAY();
void Read_Khi_Gas();
void Read_Khi_CO();
//void PWM_Config(int goc);
void GPIO_Configure();
//int Map(int x, int inMin, int inMax, int outMin, int outMax);