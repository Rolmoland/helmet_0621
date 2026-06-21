#include "max30102_i2c.h"

static void MAX30102_SDA_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin = MAX30102_IIC_SDA_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(MAX30102_IIC_PORT, &GPIO_InitStructure);
}

static void MAX30102_SDA_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin = MAX30102_IIC_SDA_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(MAX30102_IIC_PORT, &GPIO_InitStructure);
}

void MAX30102_IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStructure.Pin = MAX30102_IIC_SCL_PIN | MAX30102_IIC_SDA_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(MAX30102_IIC_PORT, &GPIO_InitStructure);

    MAX30102_IIC_SCL_H();
    MAX30102_IIC_SDA_H();
}

void MAX30102_IIC_Start(void)
{
    MAX30102_SDA_OUT();
    MAX30102_IIC_SDA_H();
    MAX30102_IIC_SCL_H();
    Delay_us(4);
    MAX30102_IIC_SDA_L();
    Delay_us(4);
    MAX30102_IIC_SCL_L();
}

void MAX30102_IIC_Stop(void)
{
    MAX30102_SDA_OUT();
    MAX30102_IIC_SCL_L();
    MAX30102_IIC_SDA_L();
    Delay_us(4);
    MAX30102_IIC_SCL_H();
    MAX30102_IIC_SDA_H();
    Delay_us(4);
}

uint8_t MAX30102_IIC_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    MAX30102_SDA_IN();
    MAX30102_IIC_SDA_H();
    Delay_us(1);
    MAX30102_IIC_SCL_H();
    Delay_us(1);
    while (MAX30102_READ_SDA)
    {
        ucErrTime++;
        if (ucErrTime > 250)
        {
            MAX30102_IIC_Stop();
            return 1;
        }
    }
    MAX30102_IIC_SCL_L();
    return 0;
}

void MAX30102_IIC_Ack(void)
{
    MAX30102_IIC_SCL_L();
    MAX30102_SDA_OUT();
    MAX30102_IIC_SDA_L();
    Delay_us(2);
    MAX30102_IIC_SCL_H();
    Delay_us(2);
    MAX30102_IIC_SCL_L();
}

void MAX30102_IIC_NAck(void)
{
    MAX30102_IIC_SCL_L();
    MAX30102_SDA_OUT();
    MAX30102_IIC_SDA_H();
    Delay_us(2);
    MAX30102_IIC_SCL_H();
    Delay_us(2);
    MAX30102_IIC_SCL_L();
}

void MAX30102_IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    MAX30102_SDA_OUT();
    MAX30102_IIC_SCL_L();
    for (t = 0; t < 8; t++)
    {
        if (txd & 0x80) MAX30102_IIC_SDA_H();
        else MAX30102_IIC_SDA_L();
        txd <<= 1;
        Delay_us(2);
        MAX30102_IIC_SCL_H();
        Delay_us(2);
        MAX30102_IIC_SCL_L();
        Delay_us(2);
    }
}

uint8_t MAX30102_IIC_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    MAX30102_SDA_IN();
    for (i = 0; i < 8; i++)
    {
        MAX30102_IIC_SCL_L();
        Delay_us(2);
        MAX30102_IIC_SCL_H();
        receive <<= 1;
        if (MAX30102_READ_SDA) receive++;
        Delay_us(1);
    }
    if (!ack) MAX30102_IIC_NAck();
    else MAX30102_IIC_Ack();
    return receive;
}

void MAX30102_IIC_WriteBytes(uint8_t WriteAddr, uint8_t *data, uint8_t dataLength)
{
    uint8_t i;
    MAX30102_IIC_Start();
    MAX30102_IIC_Send_Byte(WriteAddr);
    MAX30102_IIC_Wait_Ack();
    for (i = 0; i < dataLength; i++)
    {
        MAX30102_IIC_Send_Byte(data[i]);
        MAX30102_IIC_Wait_Ack();
    }
    MAX30102_IIC_Stop();
    HAL_Delay(10);
}

void MAX30102_IIC_ReadBytes(uint8_t deviceAddr, uint8_t writeAddr, uint8_t *data, uint8_t dataLength)
{
    uint8_t i;
    MAX30102_IIC_Start();
    MAX30102_IIC_Send_Byte(deviceAddr);
    MAX30102_IIC_Wait_Ack();
    MAX30102_IIC_Send_Byte(writeAddr);
    MAX30102_IIC_Wait_Ack();
    MAX30102_IIC_Send_Byte(deviceAddr | 0X01);
    MAX30102_IIC_Wait_Ack();
    for (i = 0; i < dataLength - 1; i++)
        data[i] = MAX30102_IIC_Read_Byte(1);
    data[dataLength - 1] = MAX30102_IIC_Read_Byte(0);
    MAX30102_IIC_Stop();
    HAL_Delay(10);
}

void MAX30102_IIC_Read_One_Byte(uint8_t daddr, uint8_t addr, uint8_t *data)
{
    MAX30102_IIC_Start();
    MAX30102_IIC_Send_Byte(daddr);
    MAX30102_IIC_Wait_Ack();
    MAX30102_IIC_Send_Byte(addr);
    MAX30102_IIC_Wait_Ack();
    MAX30102_IIC_Start();
    MAX30102_IIC_Send_Byte(daddr | 0X01);
    MAX30102_IIC_Wait_Ack();
    *data = MAX30102_IIC_Read_Byte(0);
    MAX30102_IIC_Stop();
}

void MAX30102_IIC_Write_One_Byte(uint8_t daddr, uint8_t addr, uint8_t data)
{
    MAX30102_IIC_Start();
    MAX30102_IIC_Send_Byte(daddr);
    MAX30102_IIC_Wait_Ack();
    MAX30102_IIC_Send_Byte(addr);
    MAX30102_IIC_Wait_Ack();
    MAX30102_IIC_Send_Byte(data);
    MAX30102_IIC_Wait_Ack();
    MAX30102_IIC_Stop();
    HAL_Delay(10);
}
