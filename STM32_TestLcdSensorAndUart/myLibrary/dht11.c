/*******************************************************************************
* Definitions
******************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include "stm32f10x.h"
#include "delayTimer.h"
#include "dht11.h"
/*******************************************************************************
* Variables
******************************************************************************/
uint8_t g_dataArray[5] = {0, 0 , 0 , 0, 0};
/*******************************************************************************
* Prototypes
******************************************************************************/
static void DHT11_OutPut(void);
static void DHT11_InPut(void);
static void DHT11_RST(void);
static uint8_t DHT11_Check(void);
static uint8_t DHT11_ReadBit(void);
static uint8_t DHT11_ReadByte(void);
/*******************************************************************************
* Code
******************************************************************************/
static void DHT11_OutPut(void)
{
    RCC_APB2PeriphClockCmd(DHT11_CLOCK, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruture;
    GPIO_InitStruture.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStruture.GPIO_Pin   = DHT11_PIN;
    GPIO_InitStruture.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT11_PORT, &GPIO_InitStruture);
}

static void DHT11_InPut(void)
{
    RCC_APB2PeriphClockCmd(DHT11_CLOCK, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruture;
    GPIO_InitStruture.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_InitStruture.GPIO_Pin   = DHT11_PIN;
    GPIO_InitStruture.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT11_PORT, &GPIO_InitStruture);
}

static void DHT11_RST(void)
{
    DHT11_OutPut();
    GPIO_ResetBits(DHT11_PORT, DHT11_PIN);
    DelayTimer_Ms(20);
    GPIO_SetBits(DHT11_PORT, DHT11_PIN);
    DelayTimer_Us(30);
}

static uint8_t DHT11_Check(void)
{
    DHT11_InPut();
    int time = 0;
    while ((GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN) == 1) && time < 100)
    {
        time++;
        DelayTimer_Us(1);
    }
    if (time > 80) return 1; //> 80 us la sai
    else time = 0;
    while ((GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN) == 0) && time < 100)
    {
        time++;
        DelayTimer_Us(1);
    }
    if (time > 80) return 1;
    return 0;
}

static uint8_t DHT11_ReadBit(void)
{
    int time = 0;
    while ((GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN) == 1) && time < 100)
    {
        time++;
        DelayTimer_Us(1);
    }
    time = 0;
    while ((GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN) == 0) && time < 100)
    {
        time++;
        DelayTimer_Us(1);
    }
    DelayTimer_Us(40);
    if (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN) == 1) return 1;
    else return 0;
}

static uint8_t DHT11_ReadByte(void)
{
    uint8_t data = 0;
    uint8_t i = 0;
    for (i = 0; i < 8; i++)
    {
        data  = data << 1;
        data |= DHT11_ReadBit();
    }
    return data;
}

uint8_t DHT11_Init(void)
{
    DelayTimer_Init();
    DHT11_OutPut();
    GPIO_SetBits(DHT11_PORT, DHT11_PIN);
    DHT11_RST();
    return DHT11_Check();    // de ham check tra ve 0, nghia la dung
}

bool DHT11_ReadData(uint8_t* pTemp, uint8_t* pHum)
{
    bool check;
    DHT11_RST();
    uint8_t i;
    if (DHT11_Check() == 0)
    {
        for (i = 0; i < 5; i++)
        {
            g_dataArray[i] = DHT11_ReadByte();
        }
        *pTemp = g_dataArray[2];
        *pHum = g_dataArray[0];
        check = true;
    }
    else
    {
        check = false;
    }
    return check;
}


