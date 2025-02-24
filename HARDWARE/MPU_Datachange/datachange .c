#include "datachange.h"
#include "sys.h"
#include "math.h"
#include "bsp_adc.h"


#define PRESS_MIN 0 //单位g
#define PRESS_MAX 20000

#define VOLTAGE_MIN 210
#define VOLTAGE_MAX 1000

#define CAL_PPM  10  // 校准环境中PPM值
#define RL	10  // RL阻值
//static float R0 = 8.00;


// ADC1转换的电压值通过MDA方式传到SRAM
extern __IO uint32_t ADC1_ConvertedValue[NOFCHANEL];



// 局部变量，用于保存转换计算后的值
float ADC_ConvertedValueLocal[NOFCHANEL * 2];



//串口1发送1个字符 
//c:要发送的字符
void usart1_send_char(u8 c)
{
	while((USART1->SR&0X40)==0);//等待上一次发送完毕   
	USART1->DR=c;   	
} 

//传送数据给匿名四轴上位机软件(V2.6版本)
//fun:功能字. 0XA0~0XAF
//data:数据缓存区,最多28字节!!
//len:data区有效数据个数
void usart1_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//最多28字节数据 
	send_buf[len+3]=0;	//校验数置零
	send_buf[0]=0X88;	//帧头
	send_buf[1]=fun;	//功能字
	send_buf[2]=len;	//数据长度
	for(i=0;i<len;i++)send_buf[3+i]=data[i];			//复制数据
	for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];	//计算校验和	
	for(i=0;i<len+4;i++)usart1_send_char(send_buf[i]);	//发送数据到串口1 
}

//发送加速度传感器数据和陀螺仪数据
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
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
	usart1_niming_report(0XA1,tbuf,12);//自定义帧,0XA1
}	
//通过串口1上报结算后的姿态数据给电脑
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
//roll:横滚角.单位0.01度。 -18000 -> 18000 对应 -180.00  ->  180.00度
//pitch:俯仰角.单位 0.01度。-9000 - 9000 对应 -90.00 -> 90.00 度
//yaw:航向角.单位为0.1度 0 -> 3600  对应 0 -> 360.0度
void usart1_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw)
{
	u8 tbuf[28]; 
	u8 i;
	for(i=0;i<28;i++)tbuf[i]=0;//清0
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
	usart1_niming_report(0XAF,tbuf,28);//飞控显示帧,0XAF
} 

void delay(u16 i){
	while(i--);
}

u16 Get_AD_Average(float ch,u8 times)   //求平均数
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++){
		temp_val+=ch;
		delay(5);
	}
	return temp_val/times;
} 	 


//压力数据转换公式
long map(long x,long in_min,long in_max,long out_min, long out_max){
	long rale=(x - in_min)*1000/(in_max - in_min);
	long v=rale*(out_max - out_min)+out_min;
	if(v<=0) v=0;	
	return v/1000;
	
}

//压力传感器
float Getpress(){
		uint16_t daH = 0;
		float f0,F;
		daH = (ADC1_ConvertedValue[0] & 0XFFFF0000) >> 16;	
		ADC_ConvertedValueLocal[0] = (float)daH / 4096 * 3300; //4095   单位mV
		f0 = Get_AD_Average(ADC_ConvertedValueLocal[0], 255);		
		F = map(f0, VOLTAGE_MIN, VOLTAGE_MAX, PRESS_MIN, PRESS_MAX);
		return F;	
}

	
//CO数据采集
float GetCO(){
		uint16_t daL = 0;
		float Vrl,RS,R0,CO;
		daL = (ADC1_ConvertedValue[0] & 0XFFFF);
		ADC_ConvertedValueLocal[1] = (float)daL * 3300  / 4095.f;
		Vrl = Get_AD_Average(ADC_ConvertedValueLocal[1], 255);
		RS = (3.3f - Vrl) / Vrl * RL;
		// 传感器校准函数
		R0 = RS / pow(CAL_PPM / 98.322, 1 / -1.458f);
		 // 获取传感器的值
		CO= 98.322f * pow(RS/R0, -1.458f)-10;  //减10用于修正
		return CO;
}



 

