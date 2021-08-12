#ifndef _HAL_USART_H
#define _HAL_USART_H
#include "util.h"
#include "ring_buffer.h"

// TODO: implement more usart ports
// TODO: implement all usart features (synchronous, smartcard etc.)
// TODO: take care of gpio configuration here 

#define NO_CHAR RING_BUFFER_EMPTY
#define RX_BUFFER_OK RING_BUFFER_WRITE_OK
#define RX_BUFFER_OVERRUN RING_BUFFER_WRITE_OVERRUN
#define RX_BUFFER_SIZE RING_BUFFER_SIZE

#define MODE_TX 1
#define MODE_RX 2
#define MODE_RX_TX 3

typedef struct {
	USART_TypeDef *USARTx; // USART1
	uint32_t buad;       // 115200
	uint32_t mode;      // RX_MODE|TX_MODE
} USART_Config;

void USART_Init(USART_Config *config);

void USART_WriteChar(USART_TypeDef *USARTx,char c);
void USART_Write(USART_TypeDef *USARTx,char *buff);
void USART_printf(USART_TypeDef *USARTx, char *buff, ...);

char USART_ReadChar(USART_TypeDef *USARTx);
// beware of overflows, maximum buffer write is RX_BUFFER_SIZE
uint32_t USART_ReadLineNonBlocking(USART_TypeDef *USARTx,char *buff);
uint32_t USART_ReadLine(USART_TypeDef *USARTx,char *buff);
uint32_t USART_ReadAll(USART_TypeDef *USARTx, char *buff);
uint32_t USART_Available(USART_TypeDef *USARTx);

#endif
