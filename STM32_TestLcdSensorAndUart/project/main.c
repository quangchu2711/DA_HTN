/*******************************************************************************
* Definitions
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "stm32f10x.h"
#include "delayTimer.h"
#include "lcdI2c.h"
#include "dht11.h"
#include "lightSensor.h"
#include "servo.h"
#include "uart.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define MAX_LENGTH_BUFFER (50U)
/*******************************************************************************
* Variables
******************************************************************************/
xTaskHandle sensorTask = NULL;
xTaskHandle deviceTask = NULL;
/*******************************************************************************
* Prototypes
******************************************************************************/
static void App_HandleUARTData(uint8_t data);
static void Task_TestUart(void *pvParameters);
static void Task_TestSensorAndLcd(void *pvParameters);
/*******************************************************************************
* Code
******************************************************************************/
static void Task_TestSensorAndLcd( void *pvParameters )
{
    DelayTimer_Init();
    DHT11_Init();
    LightSensor_Init();
    LCD_Init();

    uint8_t temp, hum;
    uint16_t light;
    uint32_t timeMs = 0;

    while (1)
    {
        light = LightSensor_ReadData();
        DHT11_ReadData(&temp, &hum);
        LCD_Gotoxy(0, 0);
        LCD_Printf("Sensor: %04d", light);
        LCD_Gotoxy(1, 0);
        LCD_Printf("T: %02d - H: %02d", temp, hum);
        vTaskDelay(10);
    }
}

static void Task_TestUart( void *pvParameters )
{
    LED_Init();
    uint8_t byteData;
    UART_Begin(115200);

    while (1)
    {
        if (UART_Available() > 0)
        {
            byteData = UART_Read();
            if (byteData == '1')
            {
                UART_Printf("%s\n", "Bat den");
            }
            else if (byteData == '0')
            {
                UART_Printf("%s\n", "Tat den");
            }
        }
        vTaskDelay(10);
    }
}

int main(void)
{
    xTaskCreate( Task_TestUart, "HandleSensorData", 256, NULL, 0, &sensorTask);
    xTaskCreate( Task_TestSensorAndLcd, "HandleDevice", 256, NULL, 1, &deviceTask);
    vTaskStartScheduler();
    while (1)
    {

    }
}