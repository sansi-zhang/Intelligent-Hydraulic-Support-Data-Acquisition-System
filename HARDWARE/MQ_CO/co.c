#include "co.h"

 

#define CAL_PPM  10  // У׼������PPMֵ
#define RL	10  // RL��ֵ
 
 // ��������ʼ��
void	MQ7_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct; // ���� GPIO ��ʼ���ṹ�����
    ADC_InitTypeDef ADC_InitStruct; // ���� ADC��ʼ���ṹ�����
    DMA_InitTypeDef DMA_InitStruct;
	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE); // ʹ�� GPIOA ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE); // ʹ�� ADC1 ʱ��
	
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN; // ģ������
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;	// ADCͨ������
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; // ����
    GPIO_Init(GPIOA, &GPIO_InitStruct); // ��ʼ��
	
    ADC_DeInit(ADC1);  // ��λ ADC
    ADC_StructInit(&ADC_InitStruct); // ʹ��Ĭ��ֵ��� ADC_InitStruct��Ա
    ADC_InitStruct.ADC_ContinuousConvMode = ENABLE; // ����ת��ģʽ
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right; // ���ݶ���
    ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; // ��ֹ������⣬ʹ���������
    ADC_InitStruct.ADC_ScanDirection = ADC_ScanDirection_Upward; // ��������ɨ��ģʽ (from CHSEL0 to CHSEL17)
    ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b; // 12 λģʽ
    ADC_Init(ADC1,&ADC_InitStruct); // ADC ��ʼ��
	
    ADC_ChannelConfig(ADC1,ADC_Channel_0,ADC_SampleTime_239_5Cycles); // ����ADCע��ͨ��0�����ڲ���ʱ��
	
    ADC_GetCalibrationFactor(ADC1);  // У׼ ADC
    ADC_Cmd(ADC1,ENABLE); // ADC ʹ�� 	
    ADC_StartOfConversion(ADC1); // ��ʼ ADC1 ת��
 
    DMA_DeInit(DMA1_Channel1); // ��λ
    DMA_StructInit(&DMA_InitStruct);	
    DMA_InitStruct.DMA_BufferSize = 1; // ��������ݸ���
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC; // ָ������Ϊ����Դ
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable; // �ر������洢���������
    DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)&ADC_Value; // �洢������ַ
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; // �洢��ÿ�ν��������ֽ�
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable; // �洢����ַ����
    DMA_InitStruct.DMA_Mode = DMA_Mode_Circular; // ����ѭ��ģʽ
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR; // �����ַ
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // ����ÿ�δ��������ֽ�
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // �����ַ������
    DMA_InitStruct.DMA_Priority = DMA_Priority_High; // ����ͨ��ת�����ȼ�
    DMA_Init(DMA1_Channel1, &DMA_InitStruct); // ��ʼ��
	
    ADC_DMARequestModeConfig(ADC1,ADC_DMAMode_Circular); // ʹ��ADC��DMAѭ��ת��ģʽ
	
    DMA_Cmd(DMA1_Channel1, ENABLE); //DMA ʹ��	
    ADC_DMACmd(ADC1, ENABLE); // ADC DMA ʹ��	
    ADC_StartOfConversion(ADC1); // ��ʼ ADC1 ת��
}
 
/****************************************
 *  RS/R0            ppm		*
 *  1.6		     50                 *
 *  1	             100                *
 *  0.6		     200                *
 *  0.46	     300                *
 *  0.39	     400                *
 *  0.28	     600                *
 *  0.21	     1000               *
 * ppm = 98.322f * pow(RS/R0, -1.458f)  *
 ***************************************/
static float R0;
 
 // ������У׼����
void MQ7_PPM_Calibration(float RS)
{
    R0 = RS / pow(CAL_PPM / 98.322, 1 / -1.458f);
}
 
 // ��ȡ��������ֵ
float MQ7_GetPPM(void)
{
    float Vrl = 3.3f * ADC_Value / 4095.f;
    float RS = (3.3f - Vrl) / Vrl * RL;
    if(boot_time_ms < 3000) // ��ȡϵͳִ��ʱ�䣬3sǰ����У׼
    {
	    MQ7_PPM_Calibration(RS);
    }
    float ppm = 98.322f * pow(RS/R0, -1.458f);
    return  ppm;
}