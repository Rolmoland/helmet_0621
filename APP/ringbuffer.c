#include "ringbuffer.h"

void ringbuffer_init(ringbuffer_t *rb)
{
    rb->r = 0;
    rb->w = 0;
    memset(rb->buffer, 0, sizeof(uint8_t) * RINGBUFFER_SIZE);
    rb->itemCount = 0;
}

uint8_t ringbuffer_is_full(ringbuffer_t *rb)
{
    return (rb->itemCount == RINGBUFFER_SIZE);
}

uint8_t ringbuffer_is_empty(ringbuffer_t *rb)
{
    return (rb->itemCount == 0);
}

int8_t ringbuffer_write(ringbuffer_t *rb, uint8_t *data, uint32_t num)
{
    if (ringbuffer_is_full(rb)) return -1;

    while (num--)
    {
        rb->buffer[rb->w] = *data++;
        rb->w = (rb->w + 1) % RINGBUFFER_SIZE;
        rb->itemCount++;
    }
    return 0;
}

int8_t ringbuffer_read(ringbuffer_t *rb, uint8_t *data, uint32_t num)
{
    if (ringbuffer_is_empty(rb)) return -1;

    while (num--)
    {
        *data++ = rb->buffer[rb->r];
        rb->r = (rb->r + 1) % RINGBUFFER_SIZE;
        rb->itemCount--;
    }
    return 0;
}
