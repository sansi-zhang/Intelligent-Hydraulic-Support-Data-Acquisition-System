#include "usart.h"
#include "delay.h"
#include "usart3.h"
#include "math.h"
#include "datachange.h"
#include "bsp_adc.h"
#include "sys.h"
#include "usart.h"	
#include "collect.h"

#define PRESS_MIN 0 //��λg
#define PRESS_MAX 20000

#define VOLTAGE_MIN 210
#define VOLTAGE_MAX 1000

#define CAL_PPM  10  // У׼������PPMֵ
#define RL	10  // RL��ֵ


// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern __IO uint32_t ADC1_ConvertedValue[NOFCHANEL];
extern __IO uint16_t ADC3_ConvertedValue[NOFCHANEL2];



INFO data_info;

// �ֲ����������ڱ���ת�������ĵ�ѹֵ
float ADC_ConvertedValueLocal[NOFCHANEL * 2], CO;




