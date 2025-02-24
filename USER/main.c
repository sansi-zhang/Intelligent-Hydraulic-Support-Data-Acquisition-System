#include "datachange.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "usart3.h"
#include "string.h"		
#include "TIME.h"
#include "mpu6050.h"
//����Э���
#include "onenet.h"
//�����豸
#include "esp8266.h"

//C��

#include "usmart.h"
#include "bsp_adc.h"






// �ֲ����������ڱ���ת��������ֵ
float  co,f;

float Pitch,Roll,Yaw;

int Temp;


float angle;
int temp;
float CO;
float F;


int main(void)
{	

	//���ݲɼ����ֱ���

	u8 string[10] = {0};	
	delay_init();
	uart_init(115200);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	UsartPrintf(USART_DEBUG, " system running\r\n"); 
	usart3_Init(115200);
	ESP8266_Init();					//��ʼ��ESP8266
	
	IIC_Init();
	MPU6050_initialize();     //=====MPU6050��ʼ��	
	DMP_Init();
	
	ADCx_Init();		 //��ʼ��ADC
	

	TIM2_Getsample_Int(1999,719);		//50ms����ʱ�ж�

	UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
	
	while(OneNet_DevLink())			//����OneNET
		delay_ms(500);
	
	delay_ms(250);
	while(1)
	{	
		delay_ms(999);
		//Read_DMP(&Pitch,&Roll,&Yaw);
		printf("\n\r ������=%.2f      �����=%.2f      ƫ����=%.2f \n\r",Pitch,Roll,Yaw); 
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
		OneNet_SendData();									//��������
		ESP8266_Clear();
		
		printf("OVER!!!\r\n");
	}
}
