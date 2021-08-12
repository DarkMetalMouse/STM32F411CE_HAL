#include "_HAL_USART.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

static ring_buffer USART1_RX_Buffer;

void USART1_IRQHandler(void) 
{
	if(USART1->SR & USART_SR_RXNE_Msk) {
		char c = (char)USART1->DR;
		ring_buffer_write(&USART1_RX_Buffer, (char) c);
	}
}

void USART_Init(USART_Config *config) 
{	
	if (config->USARTx == USART1)
		SET(RCC->APB2ENR,RCC_APB2ENR_USART1EN);
	else
		return;
	if (config->mode & MODE_TX)
		SET(config->USARTx->CR1,USART_CR1_TE);
	if (config->mode & MODE_RX)
		SET(config->USARTx->CR1,USART_CR1_RE);
	
	config->USARTx->BRR = SystemCoreClock / config->buad; 
	// the caclulation is actually fCK/(buad*16)
	// but since the mantissa-fractions is basicly the devider<<4 (4 bit fraction)
	// you can ignore the 16 and just store fCK/buad
	
	SET(config->USARTx->CR1, USART_CR1_RXNEIE);
	SET(config->USARTx->CR1, USART_CR1_UE);
	if (config->USARTx == USART1)
		NVIC_EnableIRQ(USART1_IRQn);
	else
		return;
}

void USART_WriteChar(USART_TypeDef *USARTx,char c)
{
	USARTx->DR = c;
	while(!(USARTx->SR & USART_SR_TXE));
}

void USART_Write(USART_TypeDef *USARTx,char *buff)
{
	uint32_t i = 0;
	while(buff[i] != '\0')
	{
		USART_WriteChar(USARTx,buff[i]);
		i++;
	}
}

void USART_printf(USART_TypeDef *USARTx, char *buff, ...)
{
	char *formatted = (char *)malloc(strlen(buff)*sizeof(char)*2/3);
	va_list args;
	va_start(args, buff);
	vsprintf(formatted,buff,args);
	USART_Write(USARTx,formatted);
	free(formatted);
	va_end(args);
}
char USART_ReadChar(USART_TypeDef *USARTx)
{
	if(USARTx == USART1) {
		return ring_buffer_read(&USART1_RX_Buffer);
	}
	return NO_CHAR;
}

uint32_t USART_ReadLineNonBlocking(USART_TypeDef *USARTx,char *buff) 
{
	if(USARTx == USART1) {
		uint32_t i;
		uint8_t newline = 0;
		for(i= 0; ring_buffer_available(&USART1_RX_Buffer) > 0 && newline != 2; i++) {
			buff[i] = ring_buffer_read(&USART1_RX_Buffer);
			if (newline == 1 && buff[i] == '\n')
				newline = 2;
			else if (buff[i] == '\r')
				newline = 1;
			else 
				newline = 0;
		}
		buff[i] = '\0';
		return i;
	}
	return 0;
}

uint32_t USART_ReadLine(USART_TypeDef *USARTx,char *buff) 
{
	if(USARTx == USART1) {
		uint32_t i = 0;
		uint8_t newline = 0;
		while (newline != 2) {
			char c = ring_buffer_read(&USART1_RX_Buffer);
			if (c != NO_CHAR) {
				buff[i++] = c;
				if (newline == 1 && c == '\n')
					newline = 2;
				else if (c == '\r')
					newline = 1;
				else 
					newline = 0;
			}
		}
		buff[i] = '\0';
		return i;
	}
	return 0;
}

uint32_t USART_ReadAll(USART_TypeDef *USARTx, char *buff) 
{
	if(USARTx == USART1) {
		uint32_t i;
		for(i= 0; ring_buffer_available(&USART1_RX_Buffer) > 0; i++) {
			buff[i] = ring_buffer_read(&USART1_RX_Buffer);
		}
		return i;
	}
	return 0;
}
uint32_t USART_Available(USART_TypeDef *USARTx)
{
	if(USARTx == USART1)
		return ring_buffer_available(&USART1_RX_Buffer);
	return 0;
}
