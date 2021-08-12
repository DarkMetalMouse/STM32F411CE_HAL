#include "_HAL_GPIO.h"


void gpio_pin_init(GPIO_Pin_Config *pin_config) {
	
	if(pin_config->pin > 15 
		|| pin_config->mode > 3 
		|| pin_config->type > 2  
		|| pin_config->speed > 3 
		|| pin_config->pull > 2)
		return;
	
	if      (pin_config->port == GPIOA)
		SET(RCC->AHB1ENR,RCC_AHB1ENR_GPIOAEN);
	else if (pin_config->port == GPIOB)
		SET(RCC->AHB1ENR,RCC_AHB1ENR_GPIOBEN);
	else if (pin_config->port == GPIOC)
		SET(RCC->AHB1ENR,RCC_AHB1ENR_GPIOCEN);
	else if (pin_config->port == GPIOD)
		SET(RCC->AHB1ENR,RCC_AHB1ENR_GPIODEN);
	else if (pin_config->port == GPIOE)
		SET(RCC->AHB1ENR,RCC_AHB1ENR_GPIOEEN);
	else if (pin_config->port == GPIOH)
		SET(RCC->AHB1ENR,RCC_AHB1ENR_GPIOHEN);
	else
		return;


	SET(pin_config->port->MODER,pin_config->mode << (pin_config->pin << 1));
	SET(pin_config->port->OTYPER, pin_config->type << pin_config->pin);
	SET(pin_config->port->OSPEEDR,pin_config->speed << (pin_config->pin << 1));
	SET(pin_config->port->PUPDR,pin_config->pull << (pin_config->pin << 1));
	
	
	SET(pin_config->port->AFR[pin_config->pin / 8],pin_config->alternate_func << (pin_config->pin%8 << 2));
	
}

void gpio_write(GPIO_TypeDef *port, uint32_t pin, uint32_t state) {
	if(state)
		port->BSRR = 1 << pin;
	else
		port->BSRR = 1 << (pin + 16);
}

void gpio_toggle(GPIO_TypeDef *port, uint32_t pin) {
	port->ODR ^= 1 << pin;
}

uint32_t gpio_read(GPIO_TypeDef *port, uint32_t pin) {
	return (port->IDR >> pin) & 1;
}

void gpio_interrupt_config(GPIO_TypeDef *port, uint32_t pin, uint32_t edge) {
	SET(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);
	
	RESET(SYSCFG->EXTICR[pin/4], 0xF << (pin % 4));
	if      (port == GPIOA)
		SET(SYSCFG->EXTICR[pin/4], 0x0 << (pin % 4));
	else if (port == GPIOB)
		SET(SYSCFG->EXTICR[pin/4], 0x1 << (pin % 4));
	else if (port == GPIOC)
		SET(SYSCFG->EXTICR[pin/4], 0x2 << (pin % 4));
	else if (port == GPIOD)
		SET(SYSCFG->EXTICR[pin/4], 0x3 << (pin % 4));
	else if (port == GPIOE)
		SET(SYSCFG->EXTICR[pin/4], 0x4 << (pin % 4));
	else if (port == GPIOH)
		SET(SYSCFG->EXTICR[pin/4], 0x7 << (pin % 4));
	else
		return;
		
	if (edge & INTERRUPT_RISING)
		SET(EXTI->RTSR,1 << pin);
	else
		RESET(EXTI->RTSR,1 << pin);
	if (edge & INTERRUPT_FALLING)
		SET(EXTI->RTSR,1 << pin);
	else
		SET(EXTI->RTSR,1 << pin);
}
void gpio_interrupt_priority(uint32_t pin, uint32_t priority) {
	if      (pin == 0)
		NVIC_SetPriority(EXTI0_IRQn, priority);
	else if (pin == 1)
		NVIC_SetPriority(EXTI1_IRQn, priority);
	else if (pin == 2)
		NVIC_SetPriority(EXTI2_IRQn, priority);
	else if (pin == 3)
		NVIC_SetPriority(EXTI3_IRQn, priority);
	else if (pin == 4)
		NVIC_SetPriority(EXTI4_IRQn, priority);
	else if (pin >= 5 && pin <= 9)
		NVIC_SetPriority(EXTI9_5_IRQn, priority);
	else if (pin >= 10 && pin <= 15)
		NVIC_SetPriority(EXTI15_10_IRQn, priority);
}
void gpio_interrupt_enable(uint32_t pin) {
	SET(EXTI->IMR, 1 << pin);
	
	if      (pin == 0)
		NVIC_EnableIRQ(EXTI0_IRQn);
	else if (pin == 1)
		NVIC_EnableIRQ(EXTI1_IRQn);
	else if (pin == 2)
		NVIC_EnableIRQ(EXTI2_IRQn);
	else if (pin == 3)
		NVIC_EnableIRQ(EXTI3_IRQn);
	else if (pin == 4)
		NVIC_EnableIRQ(EXTI4_IRQn);
	else if (pin >= 5 && pin <= 9)
		NVIC_EnableIRQ(EXTI9_5_IRQn);
	else if (pin >= 10 && pin <= 15)
		NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void gpio_interrupt_clear(uint32_t pin) {
	SET(EXTI->PR,1 << pin);
}




