/*******************************************************************************
* Definitions
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include "stm32f10x.h"
#include "delayTimer.h"
#include "lcdI2c.h"
#include "dht11.h"
#include "lightSensor.h"
#include "servo.h"
#include "uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
/*******************************************************************************
* Variables
******************************************************************************/
xTaskHandle sensorTask = NULL;
xTaskHandle deviceTask = NULL;
//xTaskHandle myTask3Hander = NULL;
//xQueueHandle xQueueKhiGas;
/*******************************************************************************
* Prototypes
******************************************************************************/
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

    while (1)
    {

    }
}

static void Task_HandleDevice( void *pvParameters )
{
    SERVO_Init(GPIOA, GPIO_Pin_6);

    while (1)
    {
    }
}

int main(void)
{
    xTaskCreate( Task_HandleSensorData, "HandleSensorData", 256, NULL, 0, &sensorTask);
    xTaskCreate( Task_HandleDevice, "HandleDevice", 256, NULL, 1, &deviceTask);

    vTaskStartScheduler();
    while (1)
    {

    }
}