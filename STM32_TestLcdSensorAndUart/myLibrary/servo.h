/*@attention on the stm32f103 kit only the following pins can be used:
    PA0, PA1, PA2, PA3
    PA6, PA7, PB0, PB1
    PB6, PB7, PB8, PB9
*/
/*Chi su dung toi da la 16 chan, tai vi su dung 4 timer, moi bo timer co 4 channel, moi
1 channel tuong ung 1 chan de dieu khien servo*/
#ifndef SERVO_H
#define SERVO_H
/*******************************************************************************
* Definitions
******************************************************************************/
void SERVO_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_X);
void SERVO_Write(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_X, int16_t pos);
#endif
