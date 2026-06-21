#include "max30102_hal.h"

uint8_t max30102_Bus_Write(uint8_t Register_Address, uint8_t Word_Data)
{
    MAX30102_IIC_Start();
    MAX30102_IIC_Send_Byte(max30102_WR_address | I2C_WR);
    if (MAX30102_IIC_Wait_Ack() != 0) { MAX30102_IIC_Stop(); return 0; }
    MAX30102_IIC_Send_Byte(Register_Address);
    if (MAX30102_IIC_Wait_Ack() != 0) { MAX30102_IIC_Stop(); return 0; }
    MAX30102_IIC_Send_Byte(Word_Data);
    if (MAX30102_IIC_Wait_Ack() != 0) { MAX30102_IIC_Stop(); return 0; }
    MAX30102_IIC_Stop();
    return 1;
}

uint8_t max30102_Bus_Read(uint8_t Register_Address)
{
    uint8_t data;
    MAX30102_IIC_Start();
    MAX30102_IIC_Send_Byte(max30102_WR_address | I2C_WR);
    if (MAX30102_IIC_Wait_Ack() != 0) { MAX30102_IIC_Stop(); return 0; }
    MAX30102_IIC_Send_Byte((uint8_t)Register_Address);
    if (MAX30102_IIC_Wait_Ack() != 0) { MAX30102_IIC_Stop(); return 0; }
    MAX30102_IIC_Start();
    MAX30102_IIC_Send_Byte(max30102_WR_address | I2C_RD);
    if (MAX30102_IIC_Wait_Ack() != 0) { MAX30102_IIC_Stop(); return 0; }
    data = MAX30102_IIC_Read_Byte(0);
    MAX30102_IIC_NAck();
    MAX30102_IIC_Stop();
    return data;
}

void max30102_FIFO_ReadBytes(uint8_t Register_Address, uint8_t *Data)
{
    max30102_Bus_Read(REG_INTR_STATUS_1);
    max30102_Bus_Read(REG_INTR_STATUS_2);

    MAX30102_IIC_Start();
    MAX30102_IIC_Send_Byte(max30102_WR_address | I2C_WR);
    if (MAX30102_IIC_Wait_Ack() != 0) { MAX30102_IIC_Stop(); return; }
    MAX30102_IIC_Send_Byte((uint8_t)Register_Address);
    if (MAX30102_IIC_Wait_Ack() != 0) { MAX30102_IIC_Stop(); return; }
    MAX30102_IIC_Start();
    MAX30102_IIC_Send_Byte(max30102_WR_address | I2C_RD);
    if (MAX30102_IIC_Wait_Ack() != 0) { MAX30102_IIC_Stop(); return; }

    Data[0] = MAX30102_IIC_Read_Byte(1);
    Data[1] = MAX30102_IIC_Read_Byte(1);
    Data[2] = MAX30102_IIC_Read_Byte(1);
    Data[3] = MAX30102_IIC_Read_Byte(1);
    Data[4] = MAX30102_IIC_Read_Byte(1);
    Data[5] = MAX30102_IIC_Read_Byte(0);
    MAX30102_IIC_Stop();
}

void MAX30102_Reset(void)
{
    max30102_Bus_Write(REG_MODE_CONFIG, 0x40);
    max30102_Bus_Write(REG_MODE_CONFIG, 0x40);
}

void MAX30102_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitStructure.Pin = MAX30102_INT_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(MAX30102_INT_PORT, &GPIO_InitStructure);

    MAX30102_IIC_Init();
    MAX30102_Reset();

    max30102_Bus_Write(REG_INTR_ENABLE_1, 0xc0);
    max30102_Bus_Write(REG_INTR_ENABLE_2, 0x00);
    max30102_Bus_Write(REG_FIFO_WR_PTR, 0x00);
    max30102_Bus_Write(REG_OVF_COUNTER, 0x00);
    max30102_Bus_Write(REG_FIFO_RD_PTR, 0x00);
    max30102_Bus_Write(REG_FIFO_CONFIG, 0x0f);
    max30102_Bus_Write(REG_MODE_CONFIG, 0x03);
    max30102_Bus_Write(REG_SPO2_CONFIG, 0x27);
    max30102_Bus_Write(REG_LED1_PA, 0x24);
    max30102_Bus_Write(REG_LED2_PA, 0x24);
    max30102_Bus_Write(REG_PILOT_PA, 0x7f);
}

void maxim_max30102_write_reg(uint8_t uch_addr, uint8_t uch_data)
{
    MAX30102_IIC_Write_One_Byte(I2C_WRITE_ADDR, uch_addr, uch_data);
}

void maxim_max30102_read_reg(uint8_t uch_addr, uint8_t *puch_data)
{
    MAX30102_IIC_Read_One_Byte(I2C_WRITE_ADDR, uch_addr, puch_data);
}

void maxim_max30102_read_fifo(uint32_t *pun_red_led, uint32_t *pun_ir_led)
{
    uint32_t un_temp;
    unsigned char uch_temp;
    char ach_i2c_data[6];
    *pun_red_led = 0;
    *pun_ir_led = 0;

    maxim_max30102_read_reg(REG_INTR_STATUS_1, &uch_temp);
    maxim_max30102_read_reg(REG_INTR_STATUS_2, &uch_temp);

    MAX30102_IIC_ReadBytes(I2C_WRITE_ADDR, REG_FIFO_DATA, (uint8_t *)ach_i2c_data, 6);

    un_temp = (unsigned char)ach_i2c_data[0];
    un_temp <<= 16;
    *pun_red_led += un_temp;
    un_temp = (unsigned char)ach_i2c_data[1];
    un_temp <<= 8;
    *pun_red_led += un_temp;
    un_temp = (unsigned char)ach_i2c_data[2];
    *pun_red_led += un_temp;

    un_temp = (unsigned char)ach_i2c_data[3];
    un_temp <<= 16;
    *pun_ir_led += un_temp;
    un_temp = (unsigned char)ach_i2c_data[4];
    un_temp <<= 8;
    *pun_ir_led += un_temp;
    un_temp = (unsigned char)ach_i2c_data[5];
    *pun_ir_led += un_temp;
    *pun_red_led &= 0x03FFFF;
    *pun_ir_led &= 0x03FFFF;
}
