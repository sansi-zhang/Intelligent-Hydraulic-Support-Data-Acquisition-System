#ifndef __COLLECT_H
#define __COLLECT_H 	


#include "usart.h"
#include "delay.h"
#include "usart3.h"
#include "math.h"
#include "datachange.h"
#include "bsp_adc.h"
#include "sys.h"
#include "usart.h"	

typedef struct {
	float temp;
	float angle;
	float co;
	float f;
}INFO;

extern INFO data_info;
void ChuankouPrint(float f0, float f1, float f2, int F0, int F1);
void collect(void);




#endif 


