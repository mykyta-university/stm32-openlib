#include "arraylist.h"
#include "handler.h"

typedef struct handler {
    void (*func)(void *);
    void *arg;
} Handler;

/*
 * Function realisation
 */

/// @brief Run handler
/// @param handler
void handler_run(Handler * handler) {
    void (*func)(void *);
    void *arg;
    func = handler->func;
    arg = handler->arg;
    (*func)(arg);
};

/// @brief Create handler
/// @param function, argument
/// @return Pointer to created handler
Handler * handler_create(void (*func)(void *), void *arg) {
    Handler * handler = malloc(sizeof(Handler));
    handler->func = func;
    handler->arg = arg;
    return handler;
};

