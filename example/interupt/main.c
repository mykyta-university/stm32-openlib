#include "button.h"
#include "led.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

void RCC_IRQHandler(void) {
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

    while (1) {}
}