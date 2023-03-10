/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include <stdbool.h>
/* Freertos includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
/* Mylib includes. */
#include "LCD_I2C.h"
#include "ADC.h"
#include "DelayTimer.h"
#include <stdio.h>
#include <math.h>
#include "Buzzer.h"
#include "Serial.h"
#include "RingBuffer.h"
#include "Servo.h"
#include "uart2.h"
#include "Button.h"

void PC13_Config(void);

ButtonManagement buttonPA6;

#define ADC_NumberChannel 2
uint16_t ADC_Value[ADC_NumberChannel];

QueueHandle_t xlogQueue;
xTaskHandle myTask1Hander = NULL;
xTaskHandle myTask2Hander = NULL;
xTaskHandle myTask3Hander = NULL;
xQueueHandle xQueueKhiGas;

//static void 

static void ADC_Read( void *pvParameters )
{
	  ADC_DMA_Configure(ADC_NumberChannel, ADC_Value);
	  uint32_t MQ2Data;
		while(1)
		{
				  MQ2Data = ADC_Value[0]*10000 + ADC_Value[1];
			    xQueueSend(xQueueKhiGas, &MQ2Data, 10);
			 		vTaskDelay(1000);			
		}
}
static void LCD_Display( void *pvParameters )
{
		UART2_Begin(9600);
		LCD_I2C_Configuration();
		LCD_Init();
		Myservo_Attach(GPIOB, GPIO_Pin_6);
	  uint32_t receiveSensorData;
		uint16_t Value1;
		uint16_t Value2;
	  float von, gas, co;
	  bool event_overThres = false;
	
	//UART2_SendString("AT\r\n"); // kiem tra module hoat dong khong
	
		while(1)
		{
					if(xQueueReceive(xQueueKhiGas, &receiveSensorData, 10))
					{
						Value1 = receiveSensorData / 10000;
						Value2 = receiveSensorData - Value1*10000;
				  	von = ((float)Value1/4095)*5;
				  	gas = ((float)26.572*exp(1.2894*von));
						co  = (float)3.027*exp((1.0698*von));
						if(gas > 132.11 && event_overThres == false)
						{
							Buzzer_Start();
							vTaskDelay(1000);
							UART2_SendString("ATD0865491023;\r\n");//goi cho bo
							event_overThres = true;
						}
						else if(gas < 132){
							Buzzer_Stop();
							event_overThres = false;
						}
						LCD_Gotoxy(0,0);
						LCD_Printf("GAS(ppm): %0.1f", gas);
						LCD_Gotoxy(2,0);
						LCD_Printf("CO(ppm): %0.1f", co);
					}
			 		vTaskDelay(500);
					if(event_overThres == true)
					{
						Myservo_Write(GPIOB, GPIO_Pin_6, 90);
					}
		}
}

static void UART_Telegram(void *pvParameters )
{
	uint8_t check = 0;
	PC13_Config();
	Serial_Begin (9600);

	Button_Config(GPIOA, GPIO_Pin_6, &buttonPA6);
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)(1));
	
	while(1)
	{
		if (Serial_Available() > 0)
		{
			uint8_t read_data = Serial_Read();
			if (read_data == '1')
			{
				GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)(0));
			}
			if (read_data == '0')
			{
				GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)(1));
			}				
		}
		if (Button_OnPress(GPIOA, GPIO_Pin_6, &buttonPA6))
		{
			check = !check;
			Serial_Printf ("%d_\n", check);
		}
		vTaskDelay(5);
	}
	
}


int main(void)
{
	SystemInit();			 
	DelayTimer_Init();
	Buzzer_Config();
	GPIO_Configure();
	
	xTaskCreate( ADC_Read, "INPUT Read ADC", 256, NULL, 0, &myTask1Hander);
	xTaskCreate( LCD_Display, "OUTPUT LCD", 256, NULL, 1, &myTask2Hander);
	xTaskCreate( UART_Telegram, "UART_Telegram", 256, NULL, 2, &myTask3Hander);
	
	xQueueKhiGas = xQueueCreate(9, sizeof(uint32_t));
	vTaskStartScheduler();
  while (1)
  {
		
  }
}

void PC13_Config(void)
{
	//Enable clock GPIOC
	RCC->APB2ENR |= (1 << 4);
	
	//PC13, mode:output, pushpull, max spedd 10Mhz
	GPIOC->CRH &= ~(0xf << 20);
	GPIOC->CRH |= (1 << 20);	
}

