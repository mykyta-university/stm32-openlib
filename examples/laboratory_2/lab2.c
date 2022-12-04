#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_adc.h"

// ініціалізація ADC
void adc_init() {
    ADC_InitTypeDef ADC_InitStructure;
    ADC_CommonInitTypeDef adc_init;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    ADC_DeInit();
    ADC_StructInit(&ADC_InitStructure);
    adc_init.ADC_Mode = ADC_Mode_Independent;
    adc_init.ADC_Prescaler = ADC_Prescaler_Div2;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_CommonInit(&amp;
    adc_init);
    ADC_Init(ADC1, &amp;
    ADC_InitStructure);
    ADC_Cmd(ADC1, ENABLE);
}

// Читаємо бмайти
u16 readADC1(u8 channel) {
    ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_3Cycles);
    ADC_SoftwareStartConv(ADC1);
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    return ADC_GetConversionValue(ADC1);
}

int main() {
    adc_init();
}