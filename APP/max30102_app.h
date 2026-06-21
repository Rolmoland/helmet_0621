#ifndef __MAX30102_APP_H__
#define __MAX30102_APP_H__

#include "bsp_system.h"

#define HEART_RATE_COMPENSATION 10
#define WINDOW_SIZE 20
#define ALPHA 0.05
#define BUFFER_LENGTH 500

typedef struct
{
    uint32_t ir_buffer[BUFFER_LENGTH];
    uint32_t red_buffer[BUFFER_LENGTH];
    int32_t spO2;
    int8_t spO2_valid;
    int32_t heart_rate;
    int8_t heart_rate_valid;
    int32_t min_value;
    int32_t max_value;
    int32_t prev_data;
    int32_t brightness;
    uint32_t buffer_length;
} MAX30102_Data;

int SmoothData(int new_value, int *buffer, int *index);
int LowPassFilter(int new_value, int previous_filtered_value);
void MAX30102_Read_Data(void);
void Calculate_Heart_Rate_and_SpO2(void);
void Update_Signal_Min_Max(void);
void Process_And_Display_Data(void);
void max30102_task(void);

extern MAX30102_Data max30102_data;

#endif
