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

/*Vi du: MAX_LENGTH_BUFFER: = 3
chuoi gui xuong: hunghung*/
#define MAX_LENGTH_BUFFER (50U)
/*******************************************************************************
* Variables
******************************************************************************/
uint8_t g_uartDataArr[MAX_LENGTH_BUFFER];
uint8_t g_idx = 0;
xTaskHandle sensorTask = NULL;
xTaskHandle deviceTask = NULL;
/*******************************************************************************
* Prototypes
******************************************************************************/
static void App_HandleUARTData(uint8_t data);
static void Task_HandleSensorData(void *pvParameters);
static void Task_HandleDevice(void *pvParameters);
/*******************************************************************************
* Code
******************************************************************************/
static void Task_HandleSensorData( void *pvParameters )
{
    DelayTimer_Init();
    DHT11_Init();
    LightSensor_Init();
    LCD_Init();

    uint8_t temp, hum;
    uint16_t light;
    uint32_t timeMs = 0;
    char msgSensor[11];

    while (1)
    {
        light = LightSensor_ReadData();
        DHT11_ReadData(&temp, &hum);
        LCD_Gotoxy(0, 0);
        LCD_Printf("Sensor: %04d", light);
        LCD_Gotoxy(1, 0);
        LCD_Printf("T: %02d - H: %02d", temp, hum);
        if (DelayTimer_GetTickMs() - timeMs > 2000)
        {
            /*23/45/589*/
            sprintf(msgSensor, "%02d/%02d/%04d", temp, hum, light);
            UART_Printf("%s\n", msgSensor);
            timeMs = DelayTimer_GetTickMs();
        }
        vTaskDelay(10);
    }
}

static void Task_HandleDevice( void *pvParameters )
{
    LED_Init();
    SERVO_Init(GPIOA, GPIO_Pin_6);
    uint8_t byteData;

    while (1)
    {
        if (UART_Available() > 0)
        {
            byteData = UART_Read();
            if (g_idx >= MAX_LENGTH_BUFFER)
            {
                g_idx = 0;
            }
            g_uartDataArr[g_idx] = byteData;
            if (byteData == '\n')
            {
                g_uartDataArr[g_idx] = '\0';
                if (strcmp((char*)g_uartDataArr, "ON1") == 0)
                {
                    LED_Set(LED_LivingRoom, true);
                }
                else if (strcmp((char*)g_uartDataArr, "OFF1") == 0)
                {
                    LED_Set(LED_LivingRoom, false);
                }
                else if (strcmp((char*)g_uartDataArr, "ON2") == 0)
                {
                    LED_Set(LED_Kitchen, true);
                }
                else if (strcmp((char*)g_uartDataArr, "OFF2") == 0)
                {
                    LED_Set(LED_Kitchen, false);
                }
                g_idx = 0;
                memset(g_uartDataArr, '\0', MAX_LENGTH_BUFFER);
            }
            else
            {
                g_idx++;
            }
        }
        vTaskDelay(10);
    }
}

int main(void)
{
    xTaskCreate( Task_HandleSensorData, "HandleSensorData", 256, NULL, 0, &sensorTask);
    xTaskCreate( Task_HandleDevice, "HandleDevice", 256, NULL, 1, &deviceTask);
    UART_Begin(115200);
    vTaskStartScheduler();
    while (1)
    {

    }
}