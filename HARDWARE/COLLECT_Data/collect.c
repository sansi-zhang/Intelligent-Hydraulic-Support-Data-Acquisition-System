#include "usart.h"
#include "delay.h"
#include "usart3.h"
#include "math.h"
#include "datachange.h"
#include "bsp_adc.h"
#include "sys.h"
#include "usart.h"	
#include "collect.h"

#define PRESS_MIN 0 //单位g
#define PRESS_MAX 20000

#define VOLTAGE_MIN 210
#define VOLTAGE_MAX 1000

#define CAL_PPM  10  // 校准环境中PPM值
#define RL	10  // RL阻值


// ADC1转换的电压值通过MDA方式传到SRAM
extern __IO uint32_t ADC1_ConvertedValue[NOFCHANEL];
extern __IO uint16_t ADC3_ConvertedValue[NOFCHANEL2];



INFO data_info;

// 局部变量，用于保存转换计算后的电压值
float ADC_ConvertedValueLocal[NOFCHANEL * 2], CO;




