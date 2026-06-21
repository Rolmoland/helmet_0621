#ifndef __UART_APP_H__
#define __UART_APP_H__

#include "bsp_system.h"

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

extern uint8_t uart_rx_index;
extern uint32_t uart_rx_ticks;
extern uint8_t uart_rx_buffer[10];

void my_printf(UART_HandleTypeDef *huart, const char *format, ...);
void uart_proc(void);

#endif

