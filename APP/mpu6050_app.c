#include "mpu6050_app.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

uint8_t i = 10;
float pitch, roll, yaw;
short aacx, aacy, aacz;
short gyrox, gyroy, gyroz;
unsigned long walk;
float steplength = 0.3, Distance;
uint8_t svm_set = 1;
uint16_t AVM;
uint16_t GVM;
bool fall_flag = 0;
bool collision_flag = 0;

uint8_t MPU_Write_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    uint8_t data[1 + len];
    data[0] = reg;
    for (uint8_t j = 0; j < len; j++)
        data[j + 1] = buf[j];

    if (HAL_I2C_Master_Transmit(&hi2c1, (addr << 1), data, len + 1, HAL_MAX_DELAY) != HAL_OK)
        return 1;
    return 0;
}

uint8_t MPU_Read_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    if (HAL_I2C_Master_Transmit(&hi2c1, (addr << 1), &reg, 1, HAL_MAX_DELAY) != HAL_OK)
        return 1;
    if (HAL_I2C_Master_Receive(&hi2c1, (addr << 1), buf, len, HAL_MAX_DELAY) != HAL_OK)
        return 1;
    return 0;
}

uint8_t MPU_Write_Byte(uint8_t reg, uint8_t data)
{
    uint8_t buf[2] = {reg, data};
    if (HAL_I2C_Master_Transmit(&hi2c1, (MPU_ADDR << 1), buf, 2, HAL_MAX_DELAY) != HAL_OK)
        return 1;
    return 0;
}

uint8_t MPU_Read_Byte(uint8_t reg)
{
    uint8_t data;
    if (HAL_I2C_Master_Transmit(&hi2c1, (MPU_ADDR << 1), &reg, 1, HAL_MAX_DELAY) != HAL_OK)
        return 0;
    if (HAL_I2C_Master_Receive(&hi2c1, (MPU_ADDR << 1), &data, 1, HAL_MAX_DELAY) != HAL_OK)
        return 0;
    return data;
}

void MPU_Init(void)
{
    MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0x00);
    MPU_Write_Byte(MPU_SAMPLE_RATE_REG, 0x07);
    MPU_Write_Byte(MPU_CFG_REG, 0x06);
    MPU_Write_Byte(MPU_GYRO_CFG_REG, 0x18);
    MPU_Write_Byte(MPU_ACCEL_CFG_REG, 0x01);
}

short MPU_Get_Temperature(void)
{
    uint8_t buf[2];
    short raw;
    float temp_val;
    MPU_Read_Len(MPU_ADDR, MPU_TEMP_OUTH_REG, 2, buf);
    raw = ((uint16_t)buf[0] << 8) | buf[1];
    temp_val = 36.53f + ((double)raw) / 340.0;
    return (short)(temp_val * 100);
}

uint8_t MPU_Get_Gyroscope(short *gx, short *gy, short *gz)
{
    uint8_t buf[6], res;
    res = MPU_Read_Len(MPU_ADDR, MPU_GYRO_XOUTH_REG, 6, buf);
    if (res == 0)
    {
        *gx = ((uint16_t)buf[0] << 8) | buf[1];
        *gy = ((uint16_t)buf[2] << 8) | buf[3];
        *gz = ((uint16_t)buf[4] << 8) | buf[5];
    }
    return res;
}

uint8_t MPU_Get_Accelerometer(short *ax, short *ay, short *az)
{
    uint8_t buf[6], res;
    res = MPU_Read_Len(MPU_ADDR, MPU_ACCEL_XOUTH_REG, 6, buf);
    if (res == 0)
    {
        *ax = ((uint16_t)buf[0] << 8) | buf[1];
        *ay = ((uint16_t)buf[2] << 8) | buf[3];
        *az = ((uint16_t)buf[4] << 8) | buf[5];
    }
    return res;
}

void mpu6050_task(void)
{
    mpu_dmp_get_data(&pitch, &roll, &yaw);
    MPU_Get_Accelerometer(&aacx, &aacy, &aacz);
    MPU_Get_Gyroscope(&gyrox, &gyroy, &gyroz);

    AVM = (uint16_t)sqrt(pow(aacx, 2) + pow(aacy, 2) + pow(aacz, 2));
    GVM = (uint16_t)sqrt(pow(gyrox, 2) + pow(gyroy, 2) + pow(gyroz, 2));

    fall_flag = ((fabs(pitch) > 60) | (fabs(roll) > 60));
}
