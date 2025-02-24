#ifndef __TIME_H
#define __TIME_H	
#include "string.h"
#include "sys.h"
#include "usart.h"
#include "mpu6050.h"
#include "datachange.h"

extern float Pitch,Roll,Yaw,co,f;
extern int Temp;


void TIM2_Getsample_Int(u16 arr,u16 psc);

#endif
