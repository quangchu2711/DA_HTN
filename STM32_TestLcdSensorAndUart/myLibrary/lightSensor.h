#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H
/*******************************************************************************
* API
******************************************************************************/
void LightSensor_Init(void);
/* Kiểu giá trị trả về uint16_t */
uint16_t LightSensor_ReadData(void);
#endif
