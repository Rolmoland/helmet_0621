#include "dht11_app.h"

void Delay_us(uint16_t us)
{
    uint32_t count = us * 21;
    while (count--) { __NOP(); }
}

void DHT11_Rst(void)
{
    DHT11_IO_OUT();
    DHT11_DQ_LOW;
    HAL_Delay(20);
    DHT11_DQ_OUT;
    Delay_us(30);
}

uint8_t DHT11_Check(void)
{
    uint8_t retry = 0;
    DHT11_IO_IN();

    while (DHT11_DQ_IN && retry < 100)
    {
        retry++;
        Delay_us(1);
    }
    if (retry >= 100) return 1;

    retry = 0;
    while (!DHT11_DQ_IN && retry < 100)
    {
        retry++;
        Delay_us(1);
    }
    if (retry >= 100) return 1;

    return 0;
}

uint8_t DHT11_Read_Bit(void)
{
    uint8_t retry = 0;

    while (DHT11_DQ_IN && retry < 100)
    {
        retry++;
        Delay_us(1);
    }

    retry = 0;
    while (!DHT11_DQ_IN && retry < 100)
    {
        retry++;
        Delay_us(1);
    }

    Delay_us(40);
    if (DHT11_DQ_IN) return 1;
    else return 0;
}

uint8_t DHT11_Read_Byte(void)
{
    uint8_t i, dat;
    dat = 0;
    for (i = 0; i < 8; i++)
    {
        dat <<= 1;
        dat |= DHT11_Read_Bit();
    }
    return dat;
}

uint8_t DHT11_Read_Data(uint8_t *temp, uint8_t *humi)
{
    uint8_t buf[5];
    uint8_t i;

    DHT11_Rst();
    if (DHT11_Check() == 0)
    {
        for (i = 0; i < 5; i++)
        {
            buf[i] = DHT11_Read_Byte();
        }

        if ((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])
        {
            *humi = buf[0];
            *temp = buf[2];
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 1;
    }
    return 0;
}

uint8_t DHT11_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);

    DHT11_Rst();
    return DHT11_Check();
}

uint8_t humi;
uint8_t temp;

void dht11_task(void)
{
    DHT11_Read_Data(&temp, &humi);
}
