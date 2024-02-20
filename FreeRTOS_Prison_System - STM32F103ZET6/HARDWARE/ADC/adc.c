//#include "adc.h"
//#include "delay.h"
////////////////////////////////////////////////////////////////////////////////////	 
//#include "math.h"
//#include "timer.h"
//static float R0=6;
//u16 adcx;
//// 精英STM32开发板
////ADC 代码	   
//								  
//////////////////////////////////////////////////////////////////////////////////// 
//	   
//		   
////初始化ADC
////这里我们仅以规则通道为例
////我们默认将开启通道0~3																	   
//void  Adc_Init(void)
//{ 	
//	ADC_InitTypeDef ADC_InitStructure; 
//	GPIO_InitTypeDef GPIO_InitStructure;

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	  //使能ADC1通道时钟
// 

//	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

//	//PA1 作为模拟通道输入引脚                         
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
//	GPIO_Init(GPIOA, &GPIO_InitStructure);	

//	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值

//	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
//	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
//	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
//	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
//	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
//	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
//	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

//  
//	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
//	
//	ADC_ResetCalibration(ADC1);	//使能复位校准  
//	 
//	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
//	
//	ADC_StartCalibration(ADC1);	 //开启AD校准
// 
//	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
// 
////	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能

//}				  
////获得ADC值
////ch:通道值 0~3
//u16 Get_Adc(u8 ch)   
//{
//  	//设置指定ADC的规则组通道，一个序列，采样时间
//	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
//  
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
//	 
//	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

//	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
//}

//u16 Get_Adc_Average(u8 ch,u8 times)
//{
//	u32 temp_val=0;
//	u8 t;
//	for(t=0;t<times;t++)
//	{
//		temp_val+=Get_Adc(ch);
//		delay_ms(5);
//	}
//	return temp_val/times;
//} 	 

#include "adc.h"
#include "delay.h"
#include "math.h"
#include "timer.h"
static float R0=6;
u16 adcx;
void Adc_Init()//初始化函数
{
		GPIO_InitTypeDef GPIO_Initstructre;
		ADC_InitTypeDef ADC_InitStruct;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC2,ENABLE);
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//
		GPIO_Initstructre.GPIO_Mode=GPIO_Mode_AIN; 
		GPIO_Initstructre.GPIO_Pin=GPIO_Pin_7;
		GPIO_Initstructre.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOA,&GPIO_Initstructre);
		GPIO_SetBits(GPIOA,GPIO_Pin_7);
		RCC_ADCCLKConfig(RCC_PCLK2_Div6);//保证不超过14M
	
		ADC_DeInit(ADC2);//复位ADC1
		ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;//不使能连续扫描
		ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//数据右对齐
		ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//软件触发，不
		ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;//独立模式
		ADC_InitStruct.ADC_NbrOfChannel = 1;
		ADC_InitStruct.ADC_ScanConvMode = DISABLE;//不使用扫描模式
	  
		ADC_Init(ADC2,&ADC_InitStruct);
	
		ADC_Cmd(ADC2,ENABLE);//使能指定的ADC1
	
		ADC_ResetCalibration(ADC2);	//使能复位校准  
	 
		while(ADC_GetResetCalibrationStatus(ADC2));	//等待复位校准结束
	
		ADC_StartCalibration(ADC2);	 //开启AD校准
 
		while(ADC_GetCalibrationStatus(ADC2));	 //等待校准结束
}
	
u16 Get_Adc(u8 ch)
{
		ADC_RegularChannelConfig(ADC2,ch,1,ADC_SampleTime_239Cycles5);//ADC1,通道1，239.5
 
		ADC_SoftwareStartConvCmd(ADC2,ENABLE);//软件复位使能
	
		while(!ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC));
	
		return ADC_GetConversionValue(ADC2);	

}

u16 Get_Adc_Average(u8 ch,u8 times)
{
		u32 temp_val=0;
		u8 t;
		for(t=0;t<times;t++)
		{
			temp_val+=Get_Adc(ch);
			delay_ms(5);
		}
		return temp_val/times;

}

void MQ2_cumlate(float RS)
{
		R0 = RS / pow(CAL_PPM / 613.9f, 1 / -2.074f);
}


float MQ2_GetPPM(void)
{   
	  float Vrl;
	float RS;
	float ppm;
	  adcx=Get_Adc_Average(ADC_Channel_9,30);//ADC1,取30次的平均值
     Vrl = 3.3f * adcx / 4096.f;
	  Vrl = ( (float)( (int)( (Vrl+0.005)*100 ) ) )/100;
     RS = (3.3f - Vrl) / Vrl * RL;
	  
    if(times<6) // 获取系统执行时间，3s前进行校准
    {
		  R0 = RS / pow(CAL_PPM / 613.9f, 1 / -2.074f);
    } 
		
	   ppm = 613.9f * pow(RS/R0, -2.074f);

    return  ppm;
}


























