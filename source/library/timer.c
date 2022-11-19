#include "timer.h"
#include "arraylist.h"
#include "misc.h"

/*
 * Global variables
 */

/*
 * Timer handler lists
 */
static ArrayList * __timer2_handler_list;

/*
 * Timer NVIC
 */
static NVIC_InitTypeDef * __timer2_nvic;

/*
 * Timer Initialization
 */
static TIM_TimeBaseInitTypeDef * __timer2_init;

/*
 * Private function realisation
 */

/// @brief Initialize NVIC for timer
/// @param NVIC_IRQChannel
void __timer_nvic_init(uint8_t NVIC_IRQChannel) {
    if (NULL == __timer2_nvic) {
        __timer2_nvic = malloc(sizeof(NVIC_InitTypeDef));
    }
    __timer2_nvic->NVIC_IRQChannel = NVIC_IRQChannel;
    __timer2_nvic->NVIC_IRQChannelPreemptionPriority = 0;
    __timer2_nvic->NVIC_IRQChannelSubPriority = 1;
    __timer2_nvic->NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(__timer2_nvic);
}

/*
 * Public function realization
 */

/// @brief Add handler to timer
/// @param timer, handler
void timer_add_handler(TIM_TypeDef * timer, Handler * handler) {
    if (__timer2_handler_list == NULL) {
        __timer2_handler_list = arraylist_create(1);
    }
    arraylist_add(__timer2_handler_list, handler);
}

/// @brief Remove all timer handlers
void timer_remove_all_handlers(TIM_TypeDef * timer) {
    if (TIMER_2 == timer) {
        arraylist_remove_all(__timer2_handler_list);
    } else {
        arraylist_remove_all(__timer2_handler_list);
    }
}

/// @brief Initialize timer interrupt
/// @param timer, prescaler, period
void timer_init_interrupt(TIM_TypeDef * timer, uint16_t prescaler, uint16_t period) {
    TIM_Cmd(TIM2, ENABLE);
    TIM_DeInit(TIM2);

    TIM_TimeBaseInitTypeDef tim_struct;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    tim_struct.TIM_Prescaler = prescaler;
    tim_struct.TIM_Period = period;
    tim_struct.TIM_ClockDivision = 0;
    tim_struct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &tim_struct);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}

void timer_set_time(TIM_TypeDef * timer, uint16_t prescaler, uint16_t period) {
    uint8_t NVIC_IRQChannel;

    if (TIMER_2 == timer) {
        NVIC_IRQChannel = TIM2_IRQn;
    } else {
        NVIC_IRQChannel = TIM2_IRQn;
    }

    __timer_nvic_init(NVIC_IRQChannel);

    TIM_Cmd(timer, DISABLE);
    TIM_DeInit(timer);

    if (NULL == __timer2_init) {
        __timer2_init = malloc(sizeof(TIM_TimeBaseInitTypeDef));
    }

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    __timer2_init->TIM_Prescaler = prescaler;
    __timer2_init->TIM_Period = period;
    __timer2_init->TIM_ClockDivision = 0;
    __timer2_init->TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(timer, __timer2_init);
    TIM_ITConfig(timer, TIM_IT_Update, ENABLE);
    TIM_Cmd(timer, ENABLE);
}

void TIM2_IRQHandler(void) {
    if (NULL == __timer2_handler_list) {
        return;
    }
    Handler *handler;
    for (int index = 0; index < arraylist_size(__timer2_handler_list); ++index) {
        handler = (Handler *) arraylist_get(__timer2_handler_list, index);
        handler_run(handler);
    }
}