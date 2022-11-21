#include <stdio.h>
#include <stdlib.h>
#include "button.h"
#include "timer.h"

void func(void * arg) {
    printf("Hello!");
}

int main() {
    Handler * handler = handler_create(&func, NULL);
    button_add_handler(handler);

    timer_add_handler(TIMER_2, handler);
    timer_set_time(TIMER_2, 10, 10);

    printf("Hello !\n");
    return EXIT_SUCCESS;
}