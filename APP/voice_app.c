#include "voice_app.h"

extern ringbuffer_t usart_rb;

bool alarm_cancel_flag = 0;
bool measure_flag = 0;
uint8_t alarm_cancel_count = 0;

static uint8_t voice_buf[64];
static uint8_t voice_idx = 0;
static uint32_t voice_last_tick = 0;

void process_asr_data(void)
{
    uint8_t ch;

    if (ringbuffer_is_empty(&usart_rb)) return;

    while (!ringbuffer_is_empty(&usart_rb) && voice_idx < 63)
    {
        ringbuffer_read(&usart_rb, &ch, 1);

        if (voice_idx == 0 && ch != 'g' && ch != 'm')
            continue;

        voice_buf[voice_idx++] = ch;
        voice_last_tick = uwTick;

        if (ch == '\n')
        {
            voice_buf[voice_idx] = '\0';

            if (strcmp((char *)voice_buf, "good\r\n") == 0)
            {
                alarm_cancel_flag = 1;
                alarm_cancel_count = 10;
            }
            else if (strcmp((char *)voice_buf, "measure\r\n") == 0)
            {
                measure_flag = 1;
            }

            voice_idx = 0;
            memset(voice_buf, 0, sizeof(voice_buf));
        }
    }

    if (voice_idx > 0 && (uwTick - voice_last_tick > 10))
    {
        voice_idx = 0;
        memset(voice_buf, 0, sizeof(voice_buf));
    }
}

void send_flag_to_asr(void)
{
    if (!alarm_cancel_flag)
    {
        if (fall_flag)
            my_printf(&huart1, "fall");
        else if (density_flag)
            my_printf(&huart1, "density");
        else if (heartrate_flag)
        {
            my_printf(&huart1, "heartrate");
            heartrate_flag = 0;
            spo2_flag = 0;
        }
        else if (spo2_flag)
        {
            my_printf(&huart1, "spo2");
            heartrate_flag = 0;
            spo2_flag = 0;
        }
    }
    else
    {
        if (--alarm_cancel_count == 0)
            alarm_cancel_flag = 0;
    }
}
