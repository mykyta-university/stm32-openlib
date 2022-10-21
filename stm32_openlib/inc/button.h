#ifndef __BUTTON_H
#define __BUTTON_H

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

#define ACTIVE_BUTTON GPIO_Pin_0

// Default GPIO_InitTypeDef for button
#define BUTTON_DEFAULT_CONFIG_GPIO_Pin     ACTIVE_BUTTON
#define BUTTON_DEFAULT_CONFIG_GPIO_Mode    GPIO_Mode_IN
#define BUTTON_DEFAULT_CONFIG_GPIO_Speed   GPIO_Speed_100MHz
#define BUTTON_DEFAULT_CONFIG_GPIO_OType   GPIO_OType_PP
#define BUTTON_DEFAULT_CONFIG_GPIO_PuPd    GPIO_PuPd_NOPULL

typedef enum {
    NOT_PRESSED = 0, PRESSED = 1
} ButtonState;

/// @brief Get GPIO config and set it by default
/// @param button_config
/// @return button_config set by default
GPIO_InitTypeDef button_config_set_default(GPIO_InitTypeDef);

/// @brief Get button state
/// @param button
/// @return ButtonState (Pressed / Not pressed)
ButtonState is_button_pressed(uint32_t);

#endif