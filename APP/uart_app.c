#include "uart_app.h"

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

uint8_t uart_rx_index = 0;
uint32_t uart_rx_ticks = 0;
uint8_t uart_rx_buffer[10] = {0};

#define BUUFER_SIZE 64

ringbuffer_t usart_rb;
uint8_t usart_read_buffer[BUUFER_SIZE];

/* Formatted UART transmit */
void my_printf(UART_HandleTypeDef *huart, const char *format, ...)
{
    char buf[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);
    HAL_UART_Transmit(huart, (uint8_t *)buf, strlen(buf), HAL_MAX_DELAY);
}

/* DMA Rx complete callback: write received data to ringbuffer */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (!ringbuffer_is_full(&usart_rb))
    {
        ringbuffer_write(&usart_rb, uart_rx_dma_buffer, Size);
    }
    memset(uart_rx_dma_buffer, 0, sizeof(uart_rx_dma_buffer));
}

/* Process UART ringbuffer data */
void uart_proc(void)
{
    if (ringbuffer_is_empty(&usart_rb)) return;
    ringbuffer_read(&usart_rb, usart_read_buffer, usart_rb.itemCount);
    printf("ringbuffer data: %s\n", usart_read_buffer);
    memset(usart_read_buffer, 0, sizeof(uint8_t) * BUUFER_SIZE);
}
