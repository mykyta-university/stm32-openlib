#ifndef __HANDLER_H
#define __HANDLER_H

#include "arraylist.h"

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

void handler_add_button_handler(void (*func)(void *), void *arg) {
    Handler *handler = malloc(sizeof(Handler));
    handler->func = func;
    handler->arg = arg;
    if (__EXTI0_IRQHandlerList == NULL) {
        __EXTI0_IRQHandlerList = arraylist_create(1);
    }
    arraylist_add(__EXTI0_IRQHandlerList, handler);
}
#endif
