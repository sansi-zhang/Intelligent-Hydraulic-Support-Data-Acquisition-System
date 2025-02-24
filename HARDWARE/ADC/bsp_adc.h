#ifndef __BSP_ADC_H
#define	__BSP_ADC_H


#include "stm32f10x.h"

// 双模式时，ADC1和ADC2转换的数据都存放在ADC1的数据寄存器，
// ADC1的在低十六位，ADC2的在高十六位
// 双ADC模式的第一个ADC，必须是ADC1

//c8t6中使用PB0的ADC1的通道8、PB1的ADC2的通道9
#define    ADCx_1                           ADC1
#define    ADCx_1_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADCx_1_CLK                       RCC_APB2Periph_ADC1

#define    ADCx_1_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADCx_1_GPIO_CLK                  RCC_APB2Periph_GPIOB  
#define    ADCx_1_PORT                      GPIOB
#define    ADCx_1_PIN                       GPIO_Pin_0
#define    ADCx_1_CHANNEL                   ADC_Channel_8



// 双ADC模式的第二个ADC，必须是ADC2
#define    ADCx_2                           ADC2
#define    ADCx_2_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADCx_2_CLK                       RCC_APB2Periph_ADC2

#define    ADCx_2_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADCx_2_GPIO_CLK                  RCC_APB2Periph_GPIOB  
#define    ADCx_2_PORT                      GPIOB
#define    ADCx_2_PIN                       GPIO_Pin_1
#define    ADCx_2_CHANNEL                   ADC_Channel_9



#define    NOFCHANEL                        1

// ADC1 对应 DMA1通道1，ADC2没有DMA功能
#define    ADC_1_DMA_CHANNEL               DMA1_Channel1





void ADCx_Init(void);


#endif /* __ADC_H */

