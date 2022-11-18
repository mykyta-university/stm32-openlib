#include <malloc.h>
#include "led.h"

/*
 * Function realisation
 */

/// @brief Create default GPIO config for led
/// @return Pointer to led config set by default
GPIO_InitTypeDef * led_get_default_config() {
    GPIO_InitTypeDef * config = malloc(sizeof(GPIO_InitTypeDef));
    config->GPIO_Pin = LED_DEFAULT_CONFIG_GPIO_Pin;
    config->GPIO_Mode = LED_DEFAULT_CONFIG_GPIO_Mode;
    config->GPIO_Speed = LED_DEFAULT_CONFIG_GPIO_Speed;
    config->GPIO_OType = LED_DEFAULT_CONFIG_GPIO_OType;
    config->GPIO_PuPd = LED_DEFAULT_CONFIG_GPIO_PuPd;
    return config;
};

/// @brief Create default GPIO config for specific led
/// @param led
/// @return Pointer to led config set for specific led
GPIO_InitTypeDef * led_get_default_config_for(uint32_t led) {
    GPIO_InitTypeDef * config = malloc(sizeof(GPIO_InitTypeDef));
    config->GPIO_Pin = led;
    config->GPIO_Mode = LED_DEFAULT_CONFIG_GPIO_Mode;
    config->GPIO_Speed = LED_DEFAULT_CONFIG_GPIO_Speed;
    config->GPIO_OType = LED_DEFAULT_CONFIG_GPIO_OType;
    config->GPIO_PuPd = LED_DEFAULT_CONFIG_GPIO_PuPd;
    return config;
};