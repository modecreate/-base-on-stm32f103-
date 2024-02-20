//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STM32F103ZET6,正点原子ELITE STM32开发板,主频72MHZ，晶振12MHZ
//QDtech-TFT液晶驱动 for STM32 IO模拟
//xiao冯@ShenZhen QDtech co.,LTD
//公司网站:www.qdtft.com
//淘宝网站：http://qdtech.taobao.com
//wiki技术网站：http://www.lcdwiki.com
//我司提供技术支持，任何技术问题欢迎随时交流学习
//固话(传真) :+86 0755-23594567 
//手机:15989313508（冯工） 
//邮箱:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com 
//技术支持QQ:3002773612  3002778157
//技术交流QQ群:324828016
//创建日期:2018/08/22
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 深圳市全动电子技术有限公司 2018-2028
//All rights reserved
/****************************************************************************************************
//=========================================电源接线================================================//
//     LCD模块                STM32单片机
//      VCC          接        DC5V/3.3V      //电源
//      GND          接          GND          //电源地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为SPI总线
//     LCD模块                STM32单片机    
//       SDA         接          PB15         //液晶屏SPI总线数据写信号
//=======================================液晶屏控制线接线==========================================//
//     LCD模块 					      STM32单片机 
//       LED         接          PB9          //液晶屏背光控制信号，如果不需要控制，接5V或3.3V
//       SCK         接          PB13         //液晶屏SPI总线时钟信号
//       A0          接          PB10         //液晶屏数据/命令控制信号
//       RESET       接          PB12         //液晶屏复位控制信号
//       CS          接          PB11         //液晶屏片选控制信号
//=========================================触摸屏触接线=========================================//
//如果模块不带触摸功能或者带有触摸功能，但是不需要触摸功能，则不需要进行触摸屏接线
//	   LCD模块                STM32单片机 
//      T_IRQ        接          PC10         //触摸屏触摸中断信号
//      T_DO         接          PC2          //触摸屏SPI总线读信号
//      T_DIN        接          PC3          //触摸屏SPI总线写信号
//      T_CS         接          PC13         //触摸屏片选控制信号
//      T_CLK        接          PC0          //触摸屏SPI总线时钟信号
**************************************************************************************************/	
 /* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/	
#ifndef __LCD180_H
#define __LCD180_H		
#include "sys.h"	 
#include "stdlib.h"

//LCD重要参数集
typedef struct  
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				  //LCD ID
	u8  dir;			  //横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16	 wramcmd;		//开始写gram指令
	u16  setxcmd;		//设置x坐标指令
	u16  setycmd;		//设置y坐标指令	 
}_180lcd_dev; 	

//LCD参数
extern _180lcd_dev lcd180dev;	//管理LCD重要参数
/////////////////////////////////////用户配置区///////////////////////////////////	 
#define USE_HORIZONTAL  	 0 //定义液晶屏顺时针旋转方向 	0-0度旋转，1-90度旋转，2-180度旋转，3-270度旋转

//////////////////////////////////////////////////////////////////////////////////	  
//定义LCD的尺寸
#define LCD180_W 130
#define LCD180_H 161

//TFTLCD部分外要调用的函数		   
extern u16  POINT180_COLOR;//默认红色    
extern u16  BACK180_COLOR; //背景颜色.默认为白色

////////////////////////////////////////////////////////////////////
//-----------------LCD端口定义---------------- 
#define GPIO_TYPE  GPIOF  //GPIO组类型
#define LED      0        //背光控制引脚        PB9
#define LCD180_CS   3       //片选引脚            PB11
#define LCD180_RS   1       //寄存器/数据选择引脚 PB10 
#define LCD180_RST  2       //复位引脚            PB12


//QDtech全系列模块采用了三极管控制背光亮灭，用户也可以接PWM调节背光亮度
#define	LCD180_LED PFout(LED) //LCD背光    		 PB9
//如果使用官方库函数定义下列底层，速度将会下降到14帧每秒，建议采用我司推荐方法
//以下IO定义直接操作寄存器，快速IO操作，刷屏速率可以达到28帧每秒！ 

//GPIO置位（拉高）
#define	LCD180_CS_SET  GPIO_TYPE->BSRR=1<<LCD180_CS    //片选端口  	PB11
#define	LCD180_RS_SET	GPIO_TYPE->BSRR=1<<LCD180_RS    //数据/命令  PB10	  
#define	LCD180_RST_SET	GPIO_TYPE->BSRR=1<<LCD180_RST   //复位			  PB12

//GPIO复位（拉低）							    
#define	LCD180_CS_CLR  GPIO_TYPE->BRR=1<<LCD180_CS     //片选端口  	PB11
#define	LCD180_RS_CLR	GPIO_TYPE->BRR=1<<LCD180_RS     //数据/命令  PB10	 
#define	LCD180_RST_CLR	GPIO_TYPE->BRR=1<<LCD180_RST    //复位			  PB12

//画笔颜色
#define WHITE180       0xFFFF
#define BLACK180      	0x0000	  
#define BLUE180       	0x001F  
#define BRED180        0XF81F
#define GRED180 			 	0XFFE0
#define GBLUE180			 	0X07FF
#define RED180         0xF800
#define MAGENTA180     0xF81F
#define GREEN180       0x07E0
#define CYAN180        0x7FFF
#define YELLOW180      0xFFE0
#define BROWN180 			0XBC40 //棕色
#define BRRED180 			0XFC07 //棕红色
#define GRAY180  			0X8430 //灰色
//GUI颜色

#define DARKBLUE180      	 0X01CF	//深蓝色
#define LIGHTBLUE180      	 0X7D7C	//浅蓝色  
#define GRAYBLUE180       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN180     	0X841F //浅绿色
#define LIGHTGRAY180     0XEF5B //浅灰色(PANNEL)
#define LGRAY180 			 		0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE180      	0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE180          0X2B12 //浅棕蓝色(选择条目的反色)
	    															  
void LCD180_Init(void);
void LCD180_DisplayOn(void);
void LCD180_DisplayOff(void);
void LCD180_Clear(u16 Color);	 
void LCD180_SetCursor(u16 Xpos, u16 Ypos);
void LCD180_DrawPoint(u16 x,u16 y);//画点
u16  LCD180_ReadPoint(u16 x,u16 y); //读点
void LCD180_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD180_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   
void LCD180_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd);

u16 LCD180_RD_DATA(void);//读取LCD数据									    
void LCD180_WriteReg(u8 LCD180_Reg, u16 LCD180_RegValue);
void LCD180_WR_DATA(u8 data);
u16 LCD180_ReadReg(u8 LCD180_Reg);
void LCD180_WriteRAM_Prepare(void);
void LCD180_WriteRAM(u16 RGB_Code);
u16 LCD180_ReadRAM(void);		   
u16 LCD180_BGR2RGB(u16 c);
void LCD180_SetParam(void);
void LCD180_WriteData_16Bit(u16 Data);
void LCD180_direction(u8 direction );

//如果仍然觉得速度不够快，可以使用下面的宏定义,提高速度.
//注意要去掉lcd.c中void LCD_WR_DATA(u16 data)函数定义哦
/*
#if LCD_USE8BIT_MODEL==1//使用8位并行数据总线模式
	#define LCD_WR_DATA(data){\
	LCD_RS_SET;\
	LCD_CS_CLR;\
	DATAOUT(data);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	DATAOUT(data<<8);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	LCD_CS_SET;\
	}
	#else//使用16位并行数据总线模式
	#define LCD_WR_DATA(data){\
	LCD_RS_SET;\
	LCD_CS_CLR;\
	DATAOUT(data);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	LCD_CS_SET;\
	} 	
#endif
*/
				  		 
#endif  
	 
	 



