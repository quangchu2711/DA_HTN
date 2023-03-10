#include "uart2.h"

void UART2_Begin (uint32_t baud)
{
	USART_InitTypeDef			UART_InitStructure;
	GPIO_InitTypeDef			GPIO_InitStructure;
/*Cap clock cho USART và port su dung*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* Cau Tx mode AF_PP, Rx mode FLOATING  */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

/*Cau hinh USART*/
	UART_InitStructure.USART_BaudRate = baud;
	UART_InitStructure.USART_WordLength = USART_WordLength_8b;
	UART_InitStructure.USART_StopBits = USART_StopBits_1;
	UART_InitStructure.USART_Parity = USART_Parity_No;
	UART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	UART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &UART_InitStructure);
		
	/* Cho phep UART hoat dong */
	USART_Cmd(USART2, ENABLE);}

void UART2_SendChar(char data)
{
    
	USART2->DR = 0x00;
	USART_SendData(USART2, data);
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}

void UART2_SendString(char *Str)
{
	while(*Str)
	{
			UART_SendChar(USART2, *Str);
			Str++;
	}
}


