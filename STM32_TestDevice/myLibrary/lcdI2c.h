#ifndef LCD_I2C_H
#define LCD_I2C_H
/*******************************************************************************
* API
******************************************************************************/
void LCD_Init (void);
void LCD_Clear(void);
void LCD_Gotoxy(uint8_t x, uint8_t y);
void LCD_Printf(const char* str, ...);
#endif
