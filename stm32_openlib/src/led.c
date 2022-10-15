#ifndef __LED_H

#include "led.h"

#endif

GPIO_InitTypeDef led_config_set_default(GPIO_InitTypeDef led_config) {
    led_config.GPIO_Pin = LED_DEFAULT_CONFIG_GPIO_Pin;
    led_config.GPIO_Mode = LED_DEFAULT_CONFIG_GPIO_Mode;
    led_config.GPIO_Speed = LED_DEFAULT_CONFIG_GPIO_Speed;
    led_config.GPIO_OType = LED_DEFAULT_CONFIG_GPIO_OType;
    led_config.GPIO_PuPd = LED_DEFAULT_CONFIG_GPIO_PuPd;
    return led_config;
}

GPIO_InitTypeDef led_config_set_default_for(GPIO_InitTypeDef led_config, uint32_t led) {
    led_config.GPIO_Pin = led;
    led_config.GPIO_Mode = LED_DEFAULT_CONFIG_GPIO_Mode;
    led_config.GPIO_Speed = LED_DEFAULT_CONFIG_GPIO_Speed;
    led_config.GPIO_OType = LED_DEFAULT_CONFIG_GPIO_OType;
    led_config.GPIO_PuPd = LED_DEFAULT_CONFIG_GPIO_PuPd;
    return led_config;
}