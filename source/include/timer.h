#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "handler.h"

/*
 * Define new specified timer names
 */

#define TIMER_2 TIM2

/*
 * Public function prototypes
 */

/// @brief Add handler to timer
/// @param timer, handler
void timer_add_handler(TIM_TypeDef*, Handler*);

/// @brief Remove all timer handlers
void timer_remove_all_handlers(TIM_TypeDef*);

/// @brief Initialize timer interrupt
/// @param timer, prescaler, period
void timer_init_interrupt(TIM_TypeDef*, uint16_t, uint16_t);

#endif