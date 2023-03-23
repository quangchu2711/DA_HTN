/*******************************************************************************
* Definitions
******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "stm32f10x.h"
#include "led.h"

#define LED_CLOCK            RCC_APB2Periph_GPIOB
#define LED_PORT             GPIOB
#define LIVING_ROOM_LED_PIN  GPIO_Pin_7
#define KICHEN_LED_PIN       GPIO_Pin_6
/*******************************************************************************
* Code
******************************************************************************/
void LED_Init(void)
{
    //Config GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(LED_CLOCK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = LIVING_ROOM_LED_PIN | KICHEN_LED_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);
}

void LED_Set(LED_Room_t ledPin, bool state)
{
    uint16_t pin;
    if (ledPin == LED_LivingRoom)
    {
        pin = LIVING_ROOM_LED_PIN;
    }
    else
    {
        pin = KICHEN_LED_PIN;
    }
    GPIO_WriteBit(LED_PORT, pin, (BitAction)state);
}
