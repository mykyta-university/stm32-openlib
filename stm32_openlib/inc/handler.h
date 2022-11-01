#ifndef __HANDLER_H
#define __HANDLER_H

#include "arraylist.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"

typedef struct handler {
    void (*func)(void *);

    void *arg;
} Handler;

ArrayList *__EXTI0_IRQHandlerList;


void EXTI0_IRQHandler(void) {
    if (NULL == __EXTI0_IRQHandlerList) {
        return;
    }
    Handler *handler;
    void (*func)(void *);
    void *arg;
    for (int index = 0; index < __EXTI0_IRQHandlerList->size; ++index) {
        handler = (Handler *) arraylist_get(__EXTI0_IRQHandlerList, index);
        func = handler->func;
        arg = handler->arg;
        (*func)(arg);
    }
}

// TODO: create better name
void *ddddd(void) {
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

void handler_add_button_handler(void (*func)(void *), void *arg) {
    Handler *handler = malloc(sizeof(Handler));
    handler->func = func;
    handler->arg = arg;
    if (__EXTI0_IRQHandlerList == NULL) {
//        TODO: Call initialization or configuration function here
        ddddd();
        __EXTI0_IRQHandlerList = arraylist_create(1);
    }
    arraylist_add(__EXTI0_IRQHandlerList, handler);
}



#endif
