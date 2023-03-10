#ifndef UART_H
#define UART_H
#include <stdint.h>
#include <stdbool.h>
#include "stm32f10x.h"

void UART_Init(uint32_t baudrate);

/* pArr: chuoi dữ liệu cần gửi
length: Chieu dai cua chuoi*/
void UART_Transmit(uint8 *pArr, uint16_t length);

/* Kiem tra xm có dữ liệu từ máy tính xuống vi điều khiển hay không ? 
true: có dữ liệu gửi xuống
false: không có dữ liệu*/
bool UART_CheckAvailable(void);

/*Dọc dữ liệu gửi xuống từ máy tính gửi xuống vi điều khiển
(đọc từng byte một)*/
uint8_t UART_ReadByte(void);

#endif
