#ifndef __BUTTON_H

#include "button.h"

#endif


GPIO_InitTypeDef button_config_set_default(GPIO_InitTypeDef gpioConf) {
    gpioConf.GPIO_Pin = BUTTON_DEFAULT_CONFIG_GPIO_Pin;
    gpioConf.GPIO_Mode = BUTTON_DEFAULT_CONFIG_GPIO_Mode;
    gpioConf.GPIO_Speed = BUTTON_DEFAULT_CONFIG_GPIO_Speed;
    gpioConf.GPIO_OType = BUTTON_DEFAULT_CONFIG_GPIO_OType;
    gpioConf.GPIO_PuPd = BUTTON_DEFAULT_CONFIG_GPIO_PuPd;
    return gpioConf;
}

ButtonState is_button_pressed(uint32_t button) {
    return GPIO_ReadInputDataBit(GPIOA, button);
}