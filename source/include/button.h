#ifndef __BUTTON_H
#define __BUTTON_H

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

#include "handler.h"

/*
 * Define new specified button names
 */
#define BUTTON_ACTIVE GPIO_Pin_0

/*
 * Default GPIO_InitTypeDef for button
 */
#define BUTTON_DEFAULT_CONFIG_GPIO_Pin     BUTTON_ACTIVE
#define BUTTON_DEFAULT_CONFIG_GPIO_Mode    GPIO_Mode_IN
#define BUTTON_DEFAULT_CONFIG_GPIO_Speed   GPIO_Speed_100MHz
#define BUTTON_DEFAULT_CONFIG_GPIO_OType   GPIO_OType_PP
#define BUTTON_DEFAULT_CONFIG_GPIO_PuPd    GPIO_PuPd_NOPULL

/*
 * Make type to define button states PRESSED = BUTTON_PRESSED, NOT PRESSED = BUTTON_FREE
 */
typedef enum {
    BUTTON_FREE = 0, BUTTON_PRESSED = !BUTTON_FREE
} ButtonState;

/*
 * Public function prototypes
 */

/// @brief Create default GPIO config for button
/// @return Pointer to button config set by default
GPIO_InitTypeDef * button_get_default_config();

/// @brief Get button state
/// @param button
/// @return ButtonState (BUTTON_FREE / BUTTON_PRESSED)
ButtonState button_get_state(uint32_t);

/// @brief Add handler to button
/// @param handler
void button_add_handler(Handler * handler);

/// @brief Remove all button handlers
void button_remove_all_handlers();

/*
 * Private function prototypes
 */

/// @brief Initialize button interrupt
void __button_init_interrupt();
#endif