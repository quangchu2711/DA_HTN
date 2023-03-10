#ifndef DHT11_H
#define DHT11_H
/* Sử dụng kiểu dữ liệu uint8_t, uint16_t,.... */
// uint8_t a; (0 -> 255= 2 mũ 8 - 1)
// int a;
#include <stdint.h>
/* Su dung kieu du lieu tru hoac false*/
#include <stdbool.h>
#include "stm32f10x.h"

/* Ham khoi tao */
void DHT11_Init(void);
/*true: doc thanh cong
false: doc that bai */
/*pTemp: gia tri tra ve cua nhiet do*/
/*pHum: gia tri tra ve cua do am*/
bool DHT11_Read(uint8_t *pTem, uint8_t *pHum);

#endif
