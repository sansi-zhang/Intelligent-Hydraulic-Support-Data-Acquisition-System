#ifndef __DATACHANGE_H
#define __DATACHANGE_H 	


#include "usart.h"			  
#include "mpu6050.h"  
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 



//将姿态传感器得到的第一数据（加速度、陀螺仪原始数据）转化为角度
void usart1_send_char(u8 c);
void usart1_niming_report(u8 fun,u8*data,u8 len);
void mpu6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz);
void usart1_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw);
void delay(u16 i);
u16 Get_AD_Average(float ch,u8 times);
long map(long x,long in_min,long in_max,long out_min, long out_max);

float Getpress(void);
float GetCO(void);


#endif 


