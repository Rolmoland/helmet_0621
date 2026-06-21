#include "mq2_app.h"

extern ADC_HandleTypeDef hadc1;
extern uint32_t dma_buff[2][30];

float adc_value_mq = 0;
float voltage_mq = 0;
float RS = 0;
float RL = 4.7;
float R0 = 35.904;
float ppm = 0;
bool density_flag = 0;

void mq2_task(void)
{
    adc_value_mq = 0;

    for (uint8_t i = 0; i < 30; i++)
    {
        adc_value_mq += (float)dma_buff[0][i];
    }

    voltage_mq = (adc_value_mq / 30.0f) / 4095.0f * 3.3f;

    RS = ((5.0f - voltage_mq) / voltage_mq) * RL;

    ppm = pow((RS / (R0 * 11.5428f)), -1.5278f);

    density_flag = (ppm > 100);
}
