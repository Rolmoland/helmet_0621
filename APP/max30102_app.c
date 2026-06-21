#include "max30102_app.h"

bool heartrate_flag = 0;
bool spo2_flag = 0;

MAX30102_Data max30102_data = {
    .buffer_length = BUFFER_LENGTH,
    .min_value = 0x3FFFF,
    .max_value = 0,
    .brightness = 0
};

int hr_buffer[WINDOW_SIZE] = {0};
int spo2_buffer[WINDOW_SIZE] = {0};
int hr_index = 0, spo2_index = 0;
int prev_hr = 0, prev_spo2 = 0;

int SmoothData(int new_value, int *buffer, int *index)
{
    buffer[*index] = new_value;
    *index = (*index + 1) % WINDOW_SIZE;
    int sum = 0;
    for (int i = 0; i < WINDOW_SIZE; i++) sum += buffer[i];
    return sum / WINDOW_SIZE;
}

int LowPassFilter(int new_value, int previous_filtered_value)
{
    return (int)(ALPHA * new_value + (1 - ALPHA) * previous_filtered_value);
}

void MAX30102_Read_Data(void)
{
    uint8_t temp[6];
    for (int i = 0; i < max30102_data.buffer_length; i++)
    {
        while (MAX30102_INT == 1);
        max30102_FIFO_ReadBytes(REG_FIFO_DATA, temp);
        max30102_data.red_buffer[i] = (long)((long)((long)temp[0] & 0x03) << 16) | (long)temp[1] << 8 | (long)temp[2];
        max30102_data.ir_buffer[i] = (long)((long)((long)temp[3] & 0x03) << 16) | (long)temp[4] << 8 | (long)temp[5];
        if (max30102_data.min_value > max30102_data.red_buffer[i])
            max30102_data.min_value = max30102_data.red_buffer[i];
        if (max30102_data.max_value < max30102_data.red_buffer[i])
            max30102_data.max_value = max30102_data.red_buffer[i];
    }
    max30102_data.heart_rate += HEART_RATE_COMPENSATION;
}

void Calculate_Heart_Rate_and_SpO2(void)
{
    maxim_heart_rate_and_oxygen_saturation(max30102_data.ir_buffer, max30102_data.buffer_length,
                                           max30102_data.red_buffer, &max30102_data.spO2, &max30102_data.spO2_valid,
                                           &max30102_data.heart_rate, &max30102_data.heart_rate_valid);
}

void Update_Signal_Min_Max(void)
{
    uint32_t un_prev_data = max30102_data.red_buffer[max30102_data.buffer_length - 1];
    for (int i = 100; i < max30102_data.buffer_length; i++)
    {
        max30102_data.red_buffer[i - 100] = max30102_data.red_buffer[i];
        max30102_data.ir_buffer[i - 100] = max30102_data.ir_buffer[i];
        max30102_data.red_buffer[i - 100] = LowPassFilter(max30102_data.red_buffer[i - 100], un_prev_data);
        max30102_data.ir_buffer[i - 100] = LowPassFilter(max30102_data.ir_buffer[i - 100], un_prev_data);
        max30102_data.red_buffer[i - 100] = SmoothData(max30102_data.red_buffer[i - 100], hr_buffer, &hr_index);
        max30102_data.ir_buffer[i - 100] = SmoothData(max30102_data.ir_buffer[i - 100], spo2_buffer, &spo2_index);
        if (max30102_data.min_value > max30102_data.red_buffer[i - 100])
            max30102_data.min_value = max30102_data.red_buffer[i - 100];
        if (max30102_data.max_value < max30102_data.red_buffer[i - 100])
            max30102_data.max_value = max30102_data.red_buffer[i - 100];
    }
}

uint8_t dis_hr = 0;
uint8_t dis_spo2 = 0;

void Process_And_Display_Data(void)
{
    if (max30102_data.heart_rate_valid == 1 && max30102_data.heart_rate < 120)
    {
        dis_hr = max30102_data.heart_rate;
        dis_spo2 = max30102_data.spO2;
    }
}

void max30102_task(void)
{
    extern bool measure_flag;
    if (measure_flag == 0) return;
    measure_flag = 0;

    dis_hr = dis_spo2 = 0;

    while (dis_hr == 0 && dis_spo2 == 0)
    {
        __disable_irq();
        MAX30102_Read_Data();
        Calculate_Heart_Rate_and_SpO2();
        Update_Signal_Min_Max();
        Process_And_Display_Data();
    }
    __enable_irq();

    my_printf(&huart1, "over");
    heartrate_flag = (dis_hr < 60 || dis_hr > 100);
    spo2_flag = (dis_spo2 < 95 || dis_spo2 > 100);
}
