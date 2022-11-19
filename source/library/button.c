#include <malloc.h>

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"

#include "button.h"
#include "arraylist.h"


/*
 * Global variables
 */

static ArrayList * __button_handler_list;

/*
 * Function realisation
 */

/// @brief Create default GPIO config for button
/// @return Pointer to button config set by default
GPIO_InitTypeDef * button_get_default_config() {
    GPIO_InitTypeDef * config = malloc(sizeof(GPIO_InitTypeDef));
    config->GPIO_Pin = BUTTON_DEFAULT_CONFIG_GPIO_Pin;
    config->GPIO_Mode = BUTTON_DEFAULT_CONFIG_GPIO_Mode;
    config->GPIO_Speed = BUTTON_DEFAULT_CONFIG_GPIO_Speed;
    config->GPIO_OType = BUTTON_DEFAULT_CONFIG_GPIO_OType;
    config->GPIO_PuPd = BUTTON_DEFAULT_CONFIG_GPIO_PuPd;
    return config;
};

/// @brief Get button state
/// @param button
/// @return ButtonState (BUTTON_FREE / BUTTON_PRESSED)
ButtonState button_get_state(uint32_t button) {
    return GPIO_ReadInputDataBit(GPIOA, button);
};

/// @brief Add handler to button
/// @param handler
void button_add_handler(Handler * handler) {
    if (__button_handler_list == NULL) {
        __button_init_interrupt();
        __button_handler_list = arraylist_create(1);
    }
    arraylist_add(__button_handler_list, handler);
}

/// @brief Remove all button handlers
void button_remove_all_handlers() {
    arraylist_remove_all(__button_handler_list);
}

/*
 * Private function realisation
 */

/// @brief Initialize button interrupt
void __button_init_interrupt() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_StructInit(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // Enable Interrupt in NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * Interrupt function realization
 */

void EXTI0_IRQHandler(void) {
    if (NULL == __button_handler_list) {
        return;
    }
    Handler *handler;
    for (int index = 0; index < arraylist_size(__button_handler_list); ++index) {
        handler = (Handler *) arraylist_get(__button_handler_list, index);
        handler_run(handler);
    }
}