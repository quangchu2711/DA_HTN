#include "Servo.h"

void Myservo_Attach(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_X)
{
	switch (GPIO_Pin_X)
  {
  	case GPIO_Pin_0:
			if (GPIOx == GPIOA)
			{
				TIM_Base_Config(TIM2, PSC, PERIOD);
				TIM_PWM_Channel_Config(TIM2, CHANNEL_1);
			}
			else if (GPIOx == GPIOB)
			{
				TIM_Base_Config(TIM3, PSC, PERIOD);
				TIM_PWM_Channel_Config(TIM3, CHANNEL_3);				
			}
  		break;
  	case GPIO_Pin_1:
			if (GPIOx == GPIOA)
			{
				TIM_Base_Config(TIM2, PSC, PERIOD);
				TIM_PWM_Channel_Config(TIM2, CHANNEL_2);
			}
			else if (GPIOx == GPIOB)
			{
				TIM_Base_Config(TIM3, PSC, PERIOD);
				TIM_PWM_Channel_Config(TIM3, CHANNEL_4);				
			}  		
			break;
  	case GPIO_Pin_2:
			TIM_Base_Config(TIM2, PSC, PERIOD);
			TIM_PWM_Channel_Config(TIM2, CHANNEL_3);
  		break;
  	case GPIO_Pin_3:
			TIM_Base_Config(TIM2, PSC, PERIOD);
			TIM_PWM_Channel_Config(TIM2, CHANNEL_4);
  		break;
  	case GPIO_Pin_6:
			if (GPIOx == GPIOA)
			{
				TIM_Base_Config(TIM3, PSC, PERIOD);
				TIM_PWM_Channel_Config(TIM3, CHANNEL_1);
			}
			else if (GPIOx == GPIOB)
			{
				TIM_Base_Config(TIM4, PSC, PERIOD);
				TIM_PWM_Channel_Config(TIM4, CHANNEL_1);				
			}  		
			break;
  	case GPIO_Pin_7:
			if (GPIOx == GPIOA)
			{
				TIM_Base_Config(TIM3, PSC, PERIOD);
				TIM_PWM_Channel_Config(TIM3, CHANNEL_2);
			}
			else if (GPIOx == GPIOB)
			{
				TIM_Base_Config(TIM4, PSC, PERIOD);
				TIM_PWM_Channel_Config(TIM4, CHANNEL_2);				
			}  		
			break;
  	case GPIO_Pin_8:
			TIM_Base_Config(TIM4, PSC, PERIOD);
			TIM_PWM_Channel_Config(TIM4, CHANNEL_3);
  		break;
  	case GPIO_Pin_9:
			TIM_Base_Config(TIM4, PSC, PERIOD);
			TIM_PWM_Channel_Config(TIM4, CHANNEL_4);
  		break;
  }
}

void Myservo_Write(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_X, int16_t pos)
{
	if (pos < 0) pos = 0;
	else if (pos > 180) pos = 180;
	uint16_t compare_value = Map(pos, 0, 180, value_for_duty_cycle_0_5_ms, value_for_duty_cycle_2_5_ms);
	
	switch (GPIO_Pin_X)
  {
  	case GPIO_Pin_0:
			if (GPIOx == GPIOA)
			{
				TIM2->CCR1 = compare_value;
			}
			else if (GPIOx == GPIOB)
			{
				TIM3->CCR3 = compare_value;
			}
  		break;
  	case GPIO_Pin_1:
			if (GPIOx == GPIOA)
			{
				TIM2->CCR2 = compare_value;				
			}
			else if (GPIOx == GPIOB)
			{
				TIM3->CCR4 = compare_value;
			}  		
			break;
  	case GPIO_Pin_2:
				TIM2->CCR3 = compare_value;
  		break;
  	case GPIO_Pin_3:
				TIM2->CCR4 = compare_value;
  		break;
  	case GPIO_Pin_6:
			if (GPIOx == GPIOA)
			{
				TIM3->CCR1 = compare_value;
			}
			else if (GPIOx == GPIOB)
			{
				TIM4->CCR1 = compare_value;
			}  		
			break;
  	case GPIO_Pin_7:
			if (GPIOx == GPIOA)
			{
				TIM3->CCR2 = compare_value;
			}
			else if (GPIOx == GPIOB)
			{
				TIM4->CCR2 = compare_value;
			}  		
			break;
  	case GPIO_Pin_8:
				TIM4->CCR3 = compare_value;
  		break;
  	case GPIO_Pin_9:
				TIM4->CCR4 = compare_value;
  		break;
  }
}

uint16_t Map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
