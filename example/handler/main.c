#include "button.h"
#include "led.h"
#include "handler.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

void toggle_led(void * arg) {
    GPIO_ToggleBits(GPIOD, LED_ALL);
}

int main() {
    // Connect Periphery
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    // Init led and button
    GPIO_InitTypeDef gpioConf;
    gpioConf = button_config_set_default(gpioConf);
    GPIO_Init(GPIOA, &gpioConf);
    gpioConf = led_config_set_default(gpioConf);
    GPIO_Init(GPIOD, &gpioConf);

    void (*handler_function)(void *) = &toggle_led;

    handler_add_button_handler(handler_function, NULL);

    while (1) {}
}