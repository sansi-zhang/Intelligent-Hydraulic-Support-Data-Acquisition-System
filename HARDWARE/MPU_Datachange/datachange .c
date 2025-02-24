#include "datachange.h"
#include "sys.h"
#include "math.h"
#include "bsp_adc.h"


#define PRESS_MIN 0 //��λg
#define PRESS_MAX 20000

#define VOLTAGE_MIN 210
#define VOLTAGE_MAX 1000

#define CAL_PPM  10  // У׼������PPMֵ
#define RL	10  // RL��ֵ
//static float R0 = 8.00;


// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern __IO uint32_t ADC1_ConvertedValue[NOFCHANEL];



// �ֲ����������ڱ���ת��������ֵ
float ADC_ConvertedValueLocal[NOFCHANEL * 2];



//����1����1���ַ� 
//c:Ҫ���͵��ַ�
void usart1_send_char(u8 c)
{
	while((USART1->SR&0X40)==0);//�ȴ���һ�η������   
	USART1->DR=c;   	
} 

//�������ݸ�����������λ�����(V2.6�汾)
//fun:������. 0XA0~0XAF
//data:���ݻ�����,���28�ֽ�!!
//len:data����Ч���ݸ���
void usart1_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//���28�ֽ����� 
	send_buf[len+3]=0;	//У��������
	send_buf[0]=0X88;	//֡ͷ
	send_buf[1]=fun;	//������
	send_buf[2]=len;	//���ݳ���
	for(i=0;i<len;i++)send_buf[3+i]=data[i];			//��������
	for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];	//����У���	
	for(i=0;i<len+4;i++)usart1_send_char(send_buf[i]);	//�������ݵ�����1 
}

//���ͼ��ٶȴ��������ݺ�����������
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ
void mpu6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz)
{
	u8 tbuf[12]; 
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;
	usart1_niming_report(0XA1,tbuf,12);//�Զ���֡,0XA1
}	
//ͨ������1�ϱ���������̬���ݸ�����
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ
//roll:�����.��λ0.01�ȡ� -18000 -> 18000 ��Ӧ -180.00  ->  180.00��
//pitch:������.��λ 0.01�ȡ�-9000 - 9000 ��Ӧ -90.00 -> 90.00 ��
//yaw:�����.��λΪ0.1�� 0 -> 3600  ��Ӧ 0 -> 360.0��
void usart1_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw)
{
	u8 tbuf[28]; 
	u8 i;
	for(i=0;i<28;i++)tbuf[i]=0;//��0
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;	
	tbuf[18]=(roll>>8)&0XFF;
	tbuf[19]=roll&0XFF;
	tbuf[20]=(pitch>>8)&0XFF;
	tbuf[21]=pitch&0XFF;
	tbuf[22]=(yaw>>8)&0XFF;
	tbuf[23]=yaw&0XFF;
	usart1_niming_report(0XAF,tbuf,28);//�ɿ���ʾ֡,0XAF
} 

void delay(u16 i){
	while(i--);
}

u16 Get_AD_Average(float ch,u8 times)   //��ƽ����
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++){
		temp_val+=ch;
		delay(5);
	}
	return temp_val/times;
} 	 


//ѹ������ת����ʽ
long map(long x,long in_min,long in_max,long out_min, long out_max){
	long rale=(x - in_min)*1000/(in_max - in_min);
	long v=rale*(out_max - out_min)+out_min;
	if(v<=0) v=0;	
	return v/1000;
	
}

//ѹ��������
float Getpress(){
		uint16_t daH = 0;
		float f0,F;
		daH = (ADC1_ConvertedValue[0] & 0XFFFF0000) >> 16;	
		ADC_ConvertedValueLocal[0] = (float)daH / 4096 * 3300; //4095   ��λmV
		f0 = Get_AD_Average(ADC_ConvertedValueLocal[0], 255);		
		F = map(f0, VOLTAGE_MIN, VOLTAGE_MAX, PRESS_MIN, PRESS_MAX);
		return F;	
}

	
//CO���ݲɼ�
float GetCO(){
		uint16_t daL = 0;
		float Vrl,RS,R0,CO;
		daL = (ADC1_ConvertedValue[0] & 0XFFFF);
		ADC_ConvertedValueLocal[1] = (float)daL * 3300  / 4095.f;
		Vrl = Get_AD_Average(ADC_ConvertedValueLocal[1], 255);
		RS = (3.3f - Vrl) / Vrl * RL;
		// ������У׼����
		R0 = RS / pow(CAL_PPM / 98.322, 1 / -1.458f);
		 // ��ȡ��������ֵ
		CO= 98.322f * pow(RS/R0, -1.458f)-10;  //��10��������
		return CO;
}



 

