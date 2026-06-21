#include "scheduler.h"

extern void mpu6050_task(void);
extern void mq2_task(void);
extern void dht11_task(void);
extern void max30102_task(void);
extern void atgm336h_task(void);
extern void send_flag_to_asr(void);
extern void process_asr_data(void);
extern void esp_report1(void);
extern void esp_report2(void);

uint8_t task_num;

typedef struct
{
    void (*task_func)(void);
    uint32_t rate_ms;
    uint32_t last_run;
} task_t;

static task_t scheduler_task[] =
{
    {process_asr_data, 20, 0},
    {mpu6050_task, 100, 0},
    {mq2_task, 100, 0},
    {dht11_task, 100, 0},
    {atgm336h_task, 100, 0},
    {max30102_task, 200, 0},
    {esp_report1, 350, 0},
    {esp_report2, 1000, 0},
    {send_flag_to_asr, 1000, 0},
};

void scheduler_init(void)
{
    task_num = sizeof(scheduler_task) / sizeof(task_t);
}

void scheduler_run(void)
{
    for (uint8_t i = 0; i < task_num; i++)
    {
        uint32_t now_time = HAL_GetTick();

        if (now_time >= scheduler_task[i].rate_ms + scheduler_task[i].last_run)
        {
            scheduler_task[i].last_run = now_time;
            scheduler_task[i].task_func();
        }
    }
}
