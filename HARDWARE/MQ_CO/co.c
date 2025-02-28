#include "co.h"

 

#define CAL_PPM  10  // 校准环境中PPM值
#define RL	10  // RL阻值
 
 // 传感器初始化
void	MQ7_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct; // 定义 GPIO 初始化结构体变量
    ADC_InitTypeDef ADC_InitStruct; // 定义 ADC初始化结构体变量
    DMA_InitTypeDef DMA_InitStruct;
	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE); // 使能 GPIOA 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE); // 使能 ADC1 时钟
	
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN; // 模拟输入
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;	// ADC通道引脚
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; // 上拉
    GPIO_Init(GPIOA, &GPIO_InitStruct); // 初始化
	
    ADC_DeInit(ADC1);  // 复位 ADC
    ADC_StructInit(&ADC_InitStruct); // 使用默认值填充 ADC_InitStruct成员
    ADC_InitStruct.ADC_ContinuousConvMode = ENABLE; // 连续转换模式
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right; // 数据对齐
    ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; // 禁止触发检测，使用软件触发
    ADC_InitStruct.ADC_ScanDirection = ADC_ScanDirection_Upward; // 启动向上扫描模式 (from CHSEL0 to CHSEL17)
    ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b; // 12 位模式
    ADC_Init(ADC1,&ADC_InitStruct); // ADC 初始化
	
    ADC_ChannelConfig(ADC1,ADC_Channel_0,ADC_SampleTime_239_5Cycles); // 配置ADC注入通道0及周期采样时间
	
    ADC_GetCalibrationFactor(ADC1);  // 校准 ADC
    ADC_Cmd(ADC1,ENABLE); // ADC 使能 	
    ADC_StartOfConversion(ADC1); // 开始 ADC1 转换
 
    DMA_DeInit(DMA1_Channel1); // 复位
    DMA_StructInit(&DMA_InitStruct);	
    DMA_InitStruct.DMA_BufferSize = 1; // 整体的数据个数
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC; // 指定外设为发送源
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable; // 关闭两个存储区互相访问
    DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)&ADC_Value; // 存储区基地址
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; // 存储区每次接收两个字节
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable; // 存储区地址自增
    DMA_InitStruct.DMA_Mode = DMA_Mode_Circular; // 开启循环模式
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR; // 外设地址
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 外设每次传输两个字节
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // 外设地址不自增
    DMA_InitStruct.DMA_Priority = DMA_Priority_High; // 设置通道转换优先级
    DMA_Init(DMA1_Channel1, &DMA_InitStruct); // 初始化
	
    ADC_DMARequestModeConfig(ADC1,ADC_DMAMode_Circular); // 使能ADC的DMA循环转换模式
	
    DMA_Cmd(DMA1_Channel1, ENABLE); //DMA 使能	
    ADC_DMACmd(ADC1, ENABLE); // ADC DMA 使能	
    ADC_StartOfConversion(ADC1); // 开始 ADC1 转换
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
 
 // 传感器校准函数
void MQ7_PPM_Calibration(float RS)
{
    R0 = RS / pow(CAL_PPM / 98.322, 1 / -1.458f);
}
 
 // 获取传感器的值
float MQ7_GetPPM(void)
{
    float Vrl = 3.3f * ADC_Value / 4095.f;
    float RS = (3.3f - Vrl) / Vrl * RL;
    if(boot_time_ms < 3000) // 获取系统执行时间，3s前进行校准
    {
	    MQ7_PPM_Calibration(RS);
    }
    float ppm = 98.322f * pow(RS/R0, -1.458f);
    return  ppm;
}