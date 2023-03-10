#ifndef LCD_I2C_H
#define LCD_I2C_H
#include <stdint.h>
#include <stdbool.h>
#include "stm32f10x.h"

/* Khởi tạo cho LCD bắt đầu khởi động */
void LCD_I2C_Init(void);

/*Di chuyen den vi tri man hinh ma minh can hien thi*/
void LCD_I2C_GotoXY(uint8_t ox, uint8_t oy)

/*Truyền chuỗi muốn hiển thị*/
void LCD_I2C_Transmit(uint8 *pArr, uint16_t length);

#endif
