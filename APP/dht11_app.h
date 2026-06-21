#ifndef _DHT11_H
#define _DHT11_H

#include "bsp_system.h"

#define DHT11_IO_IN()  {GPIOA->MODER &= ~(GPIO_MODER_MODE8); GPIOA->MODER |= (0x0 << (8*2));}
#define DHT11_IO_OUT() {GPIOA->MODER &= ~(GPIO_MODER_MODE8); GPIOA->MODER |= (0x1 << (8*2));}
#define DHT11_DQ_OUT   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET)
#define DHT11_DQ_LOW   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET)
#define DHT11_DQ_IN    HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8)

void Delay_us(uint16_t us);
uint8_t DHT11_Init(void);
uint8_t DHT11_Read_Data(uint8_t *temp, uint8_t *humi);
void dht11_task(void);

#endif
