#ifndef _HAL_GPIO_H
#define _HAL_GPIO_H
#include "util.h"

#define SET(reg,mask) (reg |= (uint32_t)mask)
#define RESET(reg,mask) (reg &= ~(uint32_t)mask)

#define HIGH 1
#define LOW 0

#define INTERRUPT_RISING 0x01
#define INTERRUPT_FALLING 0x10

#define MODE_INPUT              ((uint32_t) 0x00)
#define MODE_OUTPUT             ((uint32_t) 0x01)
#define MODE_ALTERNATE_FUNCTION ((uint32_t) 0x02)
#define MODE_ANALOG             ((uint32_t) 0x03)

#define OUTPUT_PUSH_PULL        ((uint32_t) 0x00)
#define OUTOUT_OPEN_DRAIN       ((uint32_t) 0x01)

#define OUTPUT_SPEED_LOW        ((uint32_t) 0x00)
#define OUTPUT_SPEED_MEDIUM     ((uint32_t) 0x01)
#define OUTPUT_SPEED_FAST       ((uint32_t) 0x02)
#define OUTPUT_SPEED_HIGH       ((uint32_t) 0x03)

#define INPUT_NO_PULL           ((uint32_t) 0x00)
#define INPUT_PULL_UP           ((uint32_t) 0x01)
#define INPUT_PULL_DOWN         ((uint32_t) 0x02)

typedef struct {
	GPIO_TypeDef *port; // GPIOA
	uint32_t pin;       // 0
	uint32_t mode;      // OUTPUT_MODE
	uint32_t speed;     // OUTPUT_SPEED_LOW
	uint32_t type;      // OUTPUT_PUSH_PULL
	uint32_t pull;      // INPUT_NO_PULL (doesn't matter for output pins)
	uint32_t alternate_func;
} GPIO_Pin_Config;

void gpio_pin_init(GPIO_Pin_Config *pin_config);
void gpio_write(GPIO_TypeDef *port, uint32_t pin, uint32_t state);
void gpio_toggle(GPIO_TypeDef *port, uint32_t pin);
uint32_t gpio_read(GPIO_TypeDef *port, uint32_t pin);

void gpio_interrupt_config(GPIO_TypeDef *port, uint32_t pin, uint32_t edge);
void gpio_interrupt_priority(uint32_t pin, uint32_t priority);
void gpio_interrupt_enable(uint32_t pin);
void gpio_interrupt_clear(uint32_t pin);



#endif
