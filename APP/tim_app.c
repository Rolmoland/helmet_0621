#include "tim_app.h"

void pwm_set_duty(float Duty)
{
    TIM2->CCR2 = (TIM2->ARR + 1) * (Duty / 100.0f);
}

void pwm_set_frequency(int Frequency)
{
    uint32_t TIM2_CLK = 72000000;
    uint32_t ARR_Value = (TIM2_CLK / Frequency) - 1;
    TIM2->ARR = ARR_Value;
    TIM2->CCR2 = (ARR_Value + 1) * (TIM2->CCR2 / (float)(TIM2->ARR + 1));
    TIM2->EGR = TIM_EGR_UG;
}

uint32_t tim_ic_buffer[64];
uint32_t tim_ic_val = 0;
uint32_t tim_ic_temp = 0;

/* Input capture processing: calculate frequency from IC buffer */
void ic_proc(void)
{
    tim_ic_temp = 0;
    for (int i = 0; i < 64; i++)
    {
        tim_ic_temp += tim_ic_buffer[i];
    }
    tim_ic_temp /= 64;
    tim_ic_val = (int)((float)(1000.0f * 1000.0f) / (float)tim_ic_temp);
    limit_value(&tim_ic_val, 1, 500, 20000);
}
