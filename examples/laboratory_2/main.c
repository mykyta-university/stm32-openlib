#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_tim.h"
#include "source/include/led.h"
#include "source/include/timer.h"
#include "source/include/handler.h"
#include "source/include/button.h"
//#include "led.c"

//Для ШІМ, Широтно-імпульсна модуляція
const uint32_t Period = 20 - 1;//змінна періодна
const uint32_t prescaler = 1 - 1;
static uint32_t pulse = 15;


enum{
    FIRST = 1, SECOND = -1
} mode;

void initPWM(uint16_t LED)
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
    port.GPIO_Pin = LED;
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

// ініціалізація ADC
void adc_init(ADC_TypeDef* ADCx) {
    ADC_InitTypeDef ADC_InitStructure;
    ADC_CommonInitTypeDef adc_init;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2, ENABLE);  // підключаємо одразу дві переферії
    ADC_DeInit();
    ADC_StructInit(&ADC_InitStructure);
    adc_init.ADC_Mode = ADC_Mode_Independent;
    adc_init.ADC_Prescaler = ADC_Prescaler_Div2;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_CommonInit(&adc_init);
    ADC_Init(ADCx, &ADC_InitStructure);
    ADC_Cmd(ADCx, ENABLE);
}

// Читаємо байти
// Можна вибрати тепер яке ADC ініціалізувати
u16 readADC1(u8 channel) {
    ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_3Cycles);
    ADC_SoftwareStartConv(ADC1);
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    return ADC_GetConversionValue(ADC1);
}

void timer_2_interrupt_handler(void * arg) {
    led_turn_off(LED_ALL);
    if (FIRST == mode) {
        GPIO_ToggleBits(GPIOD, LED_GREEN);
    } else if (SECOND == mode) {
        GPIO_ToggleBits(GPIOD, LED_ORANGE);
    }
}

void button_interrupt_handler(void * arg) {
    mode *= -1;
    if (FIRST == mode) {
        timer_set_time(TIM2, 300, 42000);
    } else if (SECOND == mode) {
        timer_set_time(TIM2, 600, 42000);
    }
}

void Delay(uint32_t nCount) {
    while (nCount--) {}
}


int main() {
    unsigned char a;
    // ініціалізуємо 2 ADC, щоб потім не ініціалізувати
    adc_init(ADC1);
    adc_init(ADC2);
    initPWM(LED_BLUE);

    mode = FIRST;

    Handler *timer_handler = handler_create(&timer_2_interrupt_handler, NULL);
    Handler *button_handler = handler_create(&button_interrupt_handler, NULL);

    GPIO_InitTypeDef *led_config = led_get_default_config();

    GPIO_Init(GPIOD, led_config);

    timer_add_handler(TIMER_2, timer_handler);
    timer_set_time(TIMER_2, 300, 42000);

    button_add_handler(button_handler);

    while (1) {
        if (FIRST == mode) {
            // LED_GREEN, блимає 0.3 сек
            // LED_BLUE - рівень вхідного сигналу на 1 каналі АЦП
            // Яскравість контролювати with PWM на каналі
            unsigned int bin_code = readADC1(ADC_Channel_1);
            double voltage = bin_code * 2.96 / 0xfff; // 0xfff = 4095
            pulse = voltage;
            initPWM(LED_BLUE);
            Delay(100); // затримка на 100 мс
        } else if (SECOND == mode) {
            // LED_ORANGE, блимає 0.6 сек
            // LED_RED - рівень вхідного сигналу на 2 каналі АЦП
            // Яскравість контролювати with PWM на каналі
            unsigned int bin_code = readADC1(ADC_Channel_2);
            double voltage = bin_code * 2.96 / 0xfff; // 0xfff = 4095
            pulse = voltage;
            initPWM(LED_ORANGE);
            Delay(100); // затримка на 100 мс
        } else {
            // якщо помилка, то ПІЗДЄЦ
        }
    }
}
