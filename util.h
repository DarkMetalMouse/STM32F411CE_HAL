#ifndef _UTIL_H
#define _UTIL_H

#include "stm32f411xe.h"

#define SET(reg,mask) (reg |= (uint32_t)mask)
#define RESET(reg,mask) (reg &= ~(uint32_t)mask)

#define HIGH 1
#define LOW 0

#endif
