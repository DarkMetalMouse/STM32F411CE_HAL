#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#define RING_BUFFER_SIZE 256

#define RING_BUFFER_WRITE_OK 0
#define RING_BUFFER_WRITE_OVERRUN 1
#define RING_BUFFER_EMPTY 0xFF

typedef struct {
	unsigned int head;
	unsigned int tail;
	char buffer[RING_BUFFER_SIZE];
} ring_buffer;

unsigned int ring_buffer_write(ring_buffer* buff, char c);

char ring_buffer_read(ring_buffer* buff);
unsigned int ring_buffer_available(ring_buffer* buff);

#endif
