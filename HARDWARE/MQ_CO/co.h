#ifndef __CO_H
#define __CO_H	 
#include "sys.h"
#include "stm32f10x.h"
#include "math.h"
 

// 双模式时，ADC1和ADC2转换的数据都存放在ADC1的数据寄存器，
// ADC1的在低十六位，ADC2的在高十六位
// 双ADC模式的第一个ADC，必须是ADC1
#define    ADCx_1                           ADC1
#define    ADCx_1_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADCx_1_CLK                       RCC_APB2Periph_ADC1

#define    ADCx_1_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADCx_1_GPIO_CLK                  RCC_APB2Periph_GPIOC  
#define    ADCx_1_PORT                      GPIOC
#define    ADCx_1_PIN                       GPIO_Pin_1
#define    ADCx_1_CHANNEL                   ADC_Channel_11



// 双ADC模式的第二个ADC，必须是ADC2
#define    ADCx_2                           ADC2
#define    ADCx_2_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADCx_2_CLK                       RCC_APB2Periph_ADC2

#define    ADCx_2_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADCx_2_GPIO_CLK                  RCC_APB2Periph_GPIOC  
#define    ADCx_2_PORT                      GPIOC
#define    ADCx_2_PIN                       GPIO_Pin_4
#define    ADCx_2_CHANNEL                   ADC_Channel_14



#define    NOFCHANEL                        1

// ADC1 对应 DMA1通道1，ADC3对应DMA2通道5，ADC2没有DMA功能
#define    ADC_1_DMA_CHANNEL               DMA1_Channel1


//ADC3
#define    ADCx_3                           ADC3
#define    ADC_3_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define    ADC_3_CLK                       RCC_APB2Periph_ADC3
#define    ADC_3_GPIO_APBxClock_FUN         RCC_APB2PeriphClockCmd
#define    ADC_3_GPIO_CLK                   RCC_APB2Periph_GPIOC  
#define    ADC_3_PORT                       GPIOC
#define    ADC_3_PIN                       GPIO_Pin_3
#define    ADC_3_CHANNEL                   ADC_Channel_13
#define    ADC_3_DMA_CHANNEL               DMA2_Channel5

#define    NOFCHANEL2                        1



void ADCx_Init(void);




//蜂鸣器端口定义
#define BEEP PBout(8)	// BEEP,蜂鸣器接口		   

typedef struct
{
  _Bool beep;
	

} BEEP_INFO;

extern BEEP_INFO beep_info;

void BEEP_Init(void);	//初始化
		 				    
#endif

