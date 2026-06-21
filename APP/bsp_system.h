#ifndef BSP_SYSTEM_H
#define BSP_SYSTEM_H

#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"
#include "math.h"

#include "main.h"
#include "rtc.h"
#include "usart.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"

#include "filter.h"
#include "scheduler.h"
#include "ringbuffer.h"
#include "uart_app.h"
#include "adc_app.h"
#include "tim_app.h"
#include "i2c_hal.h"

#include "mq2_app.h"
#include "dht11_app.h"
#include "gps_app.h"

#include "mpu6050_app.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "dmpKey.h"
#include "dmpmap.h"

#include "max30102_i2c.h"
#include "max30102_hal.h"
#include "max30102_app.h"
#include "algorithm.h"

#include "voice_app.h"
#include "wifi_app.h"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim1;

extern uint32_t dma_buff[2][30];
extern uint16_t uart_rx_index;
extern uint32_t uart_rx_ticks;
extern uint8_t uart_rx_buffer[10];
extern uint8_t uart_rx_dma_buffer[128];

extern bool measure_flag;
extern bool density_flag;
extern bool fall_flag;
extern bool collision_flag;
extern bool heartrate_flag;
extern bool spo2_flag;

extern float ppm;
extern uint8_t humi;
extern uint8_t temp;
extern uint8_t dis_hr;
extern uint8_t dis_spo2;
extern float longitude;
extern float latitude;

#endif



