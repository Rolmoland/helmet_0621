#ifndef __MYIIC_H
#define __MYIIC_H
#include "bsp_system.h"

#define MAX30102_IIC_PORT       GPIOB
#define MAX30102_IIC_SCL_PIN    GPIO_PIN_5
#define MAX30102_IIC_SDA_PIN    GPIO_PIN_8

#define MAX30102_IIC_SCL_H()    HAL_GPIO_WritePin(MAX30102_IIC_PORT, MAX30102_IIC_SCL_PIN, GPIO_PIN_SET)
#define MAX30102_IIC_SCL_L()    HAL_GPIO_WritePin(MAX30102_IIC_PORT, MAX30102_IIC_SCL_PIN, GPIO_PIN_RESET)
#define MAX30102_IIC_SDA_H()    HAL_GPIO_WritePin(MAX30102_IIC_PORT, MAX30102_IIC_SDA_PIN, GPIO_PIN_SET)
#define MAX30102_IIC_SDA_L()    HAL_GPIO_WritePin(MAX30102_IIC_PORT, MAX30102_IIC_SDA_PIN, GPIO_PIN_RESET)
#define MAX30102_READ_SDA       HAL_GPIO_ReadPin(MAX30102_IIC_PORT, MAX30102_IIC_SDA_PIN)

#define MAX30102_INT_PORT       GPIOB
#define MAX30102_INT_PIN        GPIO_PIN_9
#define MAX30102_INT            HAL_GPIO_ReadPin(MAX30102_INT_PORT, MAX30102_INT_PIN)

void MAX30102_IIC_Init(void);
void MAX30102_IIC_Start(void);
void MAX30102_IIC_Stop(void);
void MAX30102_IIC_Send_Byte(uint8_t txd);
uint8_t MAX30102_IIC_Read_Byte(unsigned char ack);
uint8_t MAX30102_IIC_Wait_Ack(void);
void MAX30102_IIC_Ack(void);
void MAX30102_IIC_NAck(void);
void MAX30102_IIC_Write_One_Byte(uint8_t daddr, uint8_t addr, uint8_t data);
void MAX30102_IIC_Read_One_Byte(uint8_t daddr, uint8_t addr, uint8_t *data);
void MAX30102_IIC_WriteBytes(uint8_t WriteAddr, uint8_t *data, uint8_t dataLength);
void MAX30102_IIC_ReadBytes(uint8_t deviceAddr, uint8_t writeAddr, uint8_t *data, uint8_t dataLength);

#endif
