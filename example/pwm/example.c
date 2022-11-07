#include "stm32f4xx.h"
#include "led.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"

//Для ШІМ, Широтно-імпульсна модуляція
const uint32_t Period = 20 - 1;//змінна періодна
const uint32_t prescaler = 1 - 1;
const uint32_t pulse = 15;

void initPWM()
{
    GPIO_InitTypeDef port;
    TIM_TimeBaseInitTypeDef timer;
    TIM_OCInitTypeDef timerPWM;

// активація переферії
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

//  налаштування виходу PB5, PB9
    GPIO_StructInit(&port);
    port.GPIO_Mode = GPIO_Mode_AF;
    port.GPIO_Pin = LED_ALL;
    port.GPIO_Speed = GPIO_Speed_2MHz;
    port.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOD, &port);

//активація альтернативної функції вихода PB5
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_TIM3);
//активація альтернативної функції вихода PB9
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_TIM4);

//налаштування таймера
    TIM_TimeBaseStructInit(&timer);
    timer.TIM_ClockDivision = TIM_CKD_DIV1;
    timer.TIM_CounterMode = TIM_CounterMode_Up;
    timer.TIM_Prescaler = prescaler;
    timer.TIM_Period = Period;
    TIM_TimeBaseInit(TIM4, &timer);
    TIM_TimeBaseInit(TIM3, &timer);

    TIM_OCStructInit(&timerPWM);
    timerPWM.TIM_Pulse = pulse;
    timerPWM.TIM_OCMode = TIM_OCMode_PWM1;
    timerPWM.TIM_OutputState = TIM_OutputState_Enable;
    timerPWM.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC2Init(TIM3, &timerPWM);
    TIM_OC4Init(TIM4, &timerPWM);
// налаштування синхронізація
    TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_Enable);//конфігуруємо майстер
    TIM_SelectMasterSlaveMode(TIM4, TIM_MasterSlaveMode_Enable);
    TIM_SelectInputTrigger(TIM3, TIM_TS_ITR3);//конфігуруємо підлеглого
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Gated);

    TIM_SetCounter(TIM3, 10);
    TIM_Cmd(TIM3, ENABLE);
    TIM_Cmd(TIM4, ENABLE);
}

int main()
{
    initPWM();
    while(1){}
}