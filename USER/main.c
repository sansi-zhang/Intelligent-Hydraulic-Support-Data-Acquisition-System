#include "datachange.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "usart3.h"
#include "string.h"		
#include "TIME.h"
#include "mpu6050.h"
//网络协议层
#include "onenet.h"
//网络设备
#include "esp8266.h"

//C库

#include "usmart.h"
#include "bsp_adc.h"






// 局部变量，用于保存转换计算后的值
float  co,f;

float Pitch,Roll,Yaw;

int Temp;


float angle;
int temp;
float CO;
float F;


int main(void)
{	

	//数据采集部分变量

	u8 string[10] = {0};	
	delay_init();
	uart_init(115200);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	UsartPrintf(USART_DEBUG, " system running\r\n"); 
	usart3_Init(115200);
	ESP8266_Init();					//初始化ESP8266
	
	IIC_Init();
	MPU6050_initialize();     //=====MPU6050初始化	
	DMP_Init();
	
	ADCx_Init();		 //初始化ADC
	

	TIM2_Getsample_Int(1999,719);		//50ms任务定时中断

	UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
	
	while(OneNet_DevLink())			//接入OneNET
		delay_ms(500);
	
	delay_ms(250);
	while(1)
	{	
		delay_ms(999);
		//Read_DMP(&Pitch,&Roll,&Yaw);
		printf("\n\r 俯仰角=%.2f      横滚角=%.2f      偏航角=%.2f \n\r",Pitch,Roll,Yaw); 
		printf("111111\r\n");
	
		angle=Pitch;
		temp=Temp;
		F=f;
		CO=co;
		
		printf("Pitch=%f\r\n",Pitch);
		printf("Temp=%d\r\n",Temp);
		printf("angle=%f\r\n",angle);
		printf("temp=%d\r\n",temp);
		printf("F=%f\r\n",F);
		printf("CO=%f\r\n",CO);
		delay_ms(10);

		UsartPrintf(USART_DEBUG, "OneNet_SendData\r\n");
		OneNet_SendData();									//发送数据
		ESP8266_Clear();
		
		printf("OVER!!!\r\n");
	}
}
