#include "key.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//按键输入 驱动代码		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/3/06
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									   
//////////////////////////////////////////////////////////////////////////////////	 
 	    
//按键初始化函数 
//PE3和PE4 设置成输入
void KEY_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOG,ENABLE);//使能PORTA,PORTE时钟

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;//PE4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOA15
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;//PE3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOC5
	
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;//PB3
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
// 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOB13
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;//PB3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIOB13
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;//PB3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIOB13
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;//PB3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化GPIOB13
  
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.0
	
} 
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//返回值：
//0，没有任何按键按下
//KEY0_PRES，KEY0按下
//KEY1_PRES，KEY1按下
//WKUP_PRES，WK_UP按下 
//注意此函数有响应优先级,KEY0>KEY1>WK_UP!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY0==0||KEY1==0||KEY2==0||KEY3==0||KEY4==0/*||KEY5==0*/||WK_UP==1))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY0==0)return KEY0_PRES;
		else if(KEY1==0)return KEY1_PRES;
		else if(KEY2==0)return KEY2_PRES;
		else if(KEY3==0)return KEY3_PRES;
		else if(KEY4==0)return KEY4_PRES;
		//else if(KEY5==0)return KEY5_PRES;
	  else if(WK_UP==1)return WKUP_PRES;		
	}else if(KEY0==1&&KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1/*&&KEY5==1*/&&WK_UP==0)key_up=1; 	     
	return 0;// 无按键按下
}
