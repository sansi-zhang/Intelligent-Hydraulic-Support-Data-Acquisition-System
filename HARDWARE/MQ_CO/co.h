#ifndef __CO_H
#define __CO_H	 
#include "sys.h"
#include "stm32f10x.h"
#include "math.h"
 

// ˫ģʽʱ��ADC1��ADC2ת�������ݶ������ADC1�����ݼĴ�����
// ADC1���ڵ�ʮ��λ��ADC2���ڸ�ʮ��λ
// ˫ADCģʽ�ĵ�һ��ADC��������ADC1
#define    ADCx_1                           ADC1
#define    ADCx_1_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADCx_1_CLK                       RCC_APB2Periph_ADC1

#define    ADCx_1_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADCx_1_GPIO_CLK                  RCC_APB2Periph_GPIOC  
#define    ADCx_1_PORT                      GPIOC
#define    ADCx_1_PIN                       GPIO_Pin_1
#define    ADCx_1_CHANNEL                   ADC_Channel_11



// ˫ADCģʽ�ĵڶ���ADC��������ADC2
#define    ADCx_2                           ADC2
#define    ADCx_2_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADCx_2_CLK                       RCC_APB2Periph_ADC2

#define    ADCx_2_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADCx_2_GPIO_CLK                  RCC_APB2Periph_GPIOC  
#define    ADCx_2_PORT                      GPIOC
#define    ADCx_2_PIN                       GPIO_Pin_4
#define    ADCx_2_CHANNEL                   ADC_Channel_14



#define    NOFCHANEL                        1

// ADC1 ��Ӧ DMA1ͨ��1��ADC3��ӦDMA2ͨ��5��ADC2û��DMA����
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




//�������˿ڶ���
#define BEEP PBout(8)	// BEEP,�������ӿ�		   

typedef struct
{
  _Bool beep;
	

} BEEP_INFO;

extern BEEP_INFO beep_info;

void BEEP_Init(void);	//��ʼ��
		 				    
#endif

