#ifndef __HANDLER_H
#define __HANDLER_H

typedef struct handler Handler;

/*
 * Function prototypes
 */
/// @brief Run handler
/// @param handler
void handler_run(Handler * handler);

/// @brief Create handler
/// @param function, argument
/// @return Pointer to created handler
Handler * handler_create(void (*func)(void *), void *arg);

#endif