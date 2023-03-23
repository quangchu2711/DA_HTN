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
uint8_t g_uartDataArr[MAX_LENGTH_BUFFER];
uint8_t g_idx = 0;
xTaskHandle sensorTask = NULL;
xTaskHandle deviceTask = NULL;
/*******************************************************************************
* Prototypes
******************************************************************************/
static void Task_TestServo(void *pvParameters);
static void Task_TestLed(void *pvParameters);
/*******************************************************************************
* Code
******************************************************************************/
static void Task_TestServo( void *pvParameters )
{

    LED_Init();
    SERVO_Init(GPIOA, GPIO_Pin_6);

    while (1)
    {
        SERVO_Write(GPIOA, GPIO_Pin_6, 50);
        vTaskDelay(1000);
        SERVO_Write(GPIOA, GPIO_Pin_6, 150);
        vTaskDelay(1000);
    }
}

static void Task_TestLed( void *pvParameters )
{
    DelayTimer_Init();
    LED_Init();
    while (1)
    {
        LED_Set(LED_LivingRoom, true);
        LED_Set(LED_Kitchen, true);
        DelayTimer_Ms(500);
        LED_Set(LED_LivingRoom, false);
        LED_Set(LED_Kitchen, false);
        DelayTimer_Ms(100);
        vTaskDelay(10);
    }
}

int main(void)
{
    xTaskCreate( Task_TestServo, "Task_TestServo", 256, NULL, 0, &sensorTask);
    xTaskCreate( Task_TestLed, "Task_TestLed", 256, NULL, 1, &deviceTask);
    vTaskStartScheduler();

    while (1)
    {

    }
}