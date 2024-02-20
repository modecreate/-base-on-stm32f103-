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
#include "lcd180.h"
#include "stdlib.h"
#include "delay.h"	 
#include "lcd180spi.h"

	   
//管理LCD重要参数
//默认为竖屏
_180lcd_dev lcd180dev;

//画笔颜色,背景颜色
u16 POINT180_COLOR = 0x0000,BACK180_COLOR = 0xFFFF;  
u16 DeviceCode;	 

/*****************************************************************************
 * @name       :void LCD_WR_REG(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit command to the LCD screen
 * @parameters :data:Command value to be written
 * @retvalue   :None
******************************************************************************/
void LCD180_WR_REG(u8 data)
{ 
   LCD180_CS_CLR;     
	 LCD180_RS_CLR;	  
   SPIv_WriteData(data);
   LCD180_CS_SET;	
}

/*****************************************************************************
 * @name       :void LCD_WR_DATA(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit data to the LCD screen
 * @parameters :data:data value to be written
 * @retvalue   :None
******************************************************************************/
void LCD180_WR_DATA(u8 data)
{
   LCD180_CS_CLR;
	 LCD180_RS_SET;
   SPIv_WriteData(data);
   LCD180_CS_SET;
}

/*****************************************************************************
 * @name       :void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
 * @date       :2018-08-09 
 * @function   :Write data into registers
 * @parameters :LCD_Reg:Register address
                LCD_RegValue:Data to be written
 * @retvalue   :None
******************************************************************************/
void LCD180_WriteReg(u8 LCD180_Reg, u16 LCD180_RegValue)
{	
	LCD180_WR_REG(LCD180_Reg);  
	LCD180_WR_DATA(LCD180_RegValue);	    		 
}	   

/*****************************************************************************
 * @name       :void LCD_WriteRAM_Prepare(void)
 * @date       :2018-08-09 
 * @function   :Write GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void LCD180_WriteRAM_Prepare(void)
{
	LCD180_WR_REG(lcd180dev.wramcmd);
}	 

/*****************************************************************************
 * @name       :void Lcd_WriteData_16Bit(u16 Data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :Data:Data to be written
 * @retvalue   :None
******************************************************************************/	 
void LCD180_WriteData_16Bit(u16 Data)
{	
   LCD180_CS_CLR;
   LCD180_RS_SET;  
   SPIv_WriteData(Data>>8);
	 SPIv_WriteData(Data);
   LCD180_CS_SET;
}

/*****************************************************************************
 * @name       :void LCD_DrawPoint(u16 x,u16 y)
 * @date       :2018-08-09 
 * @function   :Write a pixel data at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/	
void LCD180_DrawPoint(u16 x,u16 y)
{
	LCD180_SetCursor(x,y);//设置光标位置 
	LCD180_WriteData_16Bit(POINT180_COLOR); 
}

/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color)
 * @date       :2018-08-09 
 * @function   :Full screen filled LCD screen
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/	
void LCD180_Clear(u16 Color)
{
  unsigned int i,m;  
	LCD180_SetWindows(0,0,lcd180dev.width-1,lcd180dev.height-1);   
	LCD180_CS_CLR;
	LCD180_RS_SET;
	for(i=0;i<lcd180dev.height;i++)
	{
    for(m=0;m<lcd180dev.width;m++)
    {	
			LCD180_WriteData_16Bit(Color);
		}
	}
	 LCD180_CS_SET;
} 

/*****************************************************************************
 * @name       :void LCD_GPIOInit(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen GPIO
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD180_GPIOInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	      
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOF ,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //推挽输出
	GPIO_Init(GPIOF, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOF, &GPIO_InitStructure); 
}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09 
 * @function   :Reset LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD180_RESET(void)
{
	LCD180_RST_CLR;
	delay_ms(100);	
	LCD180_RST_SET;
	delay_ms(50);
}

/*****************************************************************************
 * @name       :void LCD_Init(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 	 
void LCD180_Init(void)
{  
	LCD180_GPIOInit();//LCD180 GPIO初始化	
 	LCD180_RESET(); //LCD180 复位
//************* ST7735S初始化**********//	
	LCD180_WR_REG(0x11);//Sleep exit 
	delay_ms (120);	
	//ST7735R Frame Rate
	LCD180_WR_REG(0xB1); 
	LCD180_WR_DATA(0x01); 
	LCD180_WR_DATA(0x2C); 
	LCD180_WR_DATA(0x2D); 
	LCD180_WR_REG(0xB2); 
	LCD180_WR_DATA(0x01); 
	LCD180_WR_DATA(0x2C); 
	LCD180_WR_DATA(0x2D); 
	LCD180_WR_REG(0xB3); 
	LCD180_WR_DATA(0x01); 
	LCD180_WR_DATA(0x2C); 
	LCD180_WR_DATA(0x2D); 
	LCD180_WR_DATA(0x01); 
	LCD180_WR_DATA(0x2C); 
	LCD180_WR_DATA(0x2D); 	
	LCD180_WR_REG(0xB4); //Column inversion 
	LCD180_WR_DATA(0x07); 	
	//ST7735R Power Sequence
	LCD180_WR_REG(0xC0); 
	LCD180_WR_DATA(0xA2); 
	LCD180_WR_DATA(0x02); 
	LCD180_WR_DATA(0x84); 
	LCD180_WR_REG(0xC1); 
	LCD180_WR_DATA(0xC5); 
	LCD180_WR_REG(0xC2); 
	LCD180_WR_DATA(0x0A); 
	LCD180_WR_DATA(0x00); 
	LCD180_WR_REG(0xC3); 
	LCD180_WR_DATA(0x8A); 
	LCD180_WR_DATA(0x2A); 
	LCD180_WR_REG(0xC4); 
	LCD180_WR_DATA(0x8A); 
	LCD180_WR_DATA(0xEE); 
	LCD180_WR_REG(0xC5); //VCOM 
	LCD180_WR_DATA(0x0E); 	
	LCD180_WR_REG(0x36); //MX, MY, RGB mode 
	LCD180_WR_DATA(0xC0); 
	//ST7735R Gamma Sequence
	LCD180_WR_REG(0xe0); 
	LCD180_WR_DATA(0x0f); 
	LCD180_WR_DATA(0x1a); 
	LCD180_WR_DATA(0x0f); 
	LCD180_WR_DATA(0x18); 
	LCD180_WR_DATA(0x2f); 
	LCD180_WR_DATA(0x28); 
	LCD180_WR_DATA(0x20); 
	LCD180_WR_DATA(0x22); 
	LCD180_WR_DATA(0x1f); 
	LCD180_WR_DATA(0x1b); 
	LCD180_WR_DATA(0x23); 
	LCD180_WR_DATA(0x37); 
	LCD180_WR_DATA(0x00); 	
	LCD180_WR_DATA(0x07); 
	LCD180_WR_DATA(0x02); 
	LCD180_WR_DATA(0x10); 
	LCD180_WR_REG(0xe1); 
	LCD180_WR_DATA(0x0f); 
	LCD180_WR_DATA(0x1b); 
	LCD180_WR_DATA(0x0f); 
	LCD180_WR_DATA(0x17); 
	LCD180_WR_DATA(0x33); 
	LCD180_WR_DATA(0x2c); 
	LCD180_WR_DATA(0x29); 
	LCD180_WR_DATA(0x2e); 
	LCD180_WR_DATA(0x30); 
	LCD180_WR_DATA(0x30); 
	LCD180_WR_DATA(0x39); 
	LCD180_WR_DATA(0x3f); 
	LCD180_WR_DATA(0x00); 
	LCD180_WR_DATA(0x07); 
	LCD180_WR_DATA(0x03); 
	LCD180_WR_DATA(0x10);  	
	LCD180_WR_REG(0x2a);
	LCD180_WR_DATA(0x00);
	LCD180_WR_DATA(0x00);
	LCD180_WR_DATA(0x00);
	LCD180_WR_DATA(0x7f);
	LCD180_WR_REG(0x2b);
	LCD180_WR_DATA(0x00);
	LCD180_WR_DATA(0x00);
	LCD180_WR_DATA(0x00);
	LCD180_WR_DATA(0x9f);
	LCD180_WR_REG(0xF0); //Enable test command  
	LCD180_WR_DATA(0x01); 
	LCD180_WR_REG(0xF6); //Disable ram power save mode 
	LCD180_WR_DATA(0x00); 	
	LCD180_WR_REG(0x3A); //65k mode 
	LCD180_WR_DATA(0x05); 
	LCD180_WR_REG(0x29);//Display on	 	
  LCD180_direction(USE_HORIZONTAL);//设置LCD显示方向
	LCD180_LED=1;//点亮背光	 
	LCD180_Clear(WHITE180);//清全屏白色
}
 
/*****************************************************************************
 * @name       :void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
 * @date       :2018-08-09 
 * @function   :Setting LCD display window
 * @parameters :xStar:the bebinning x coordinate of the LCD display window
								yStar:the bebinning y coordinate of the LCD display window
								xEnd:the endning x coordinate of the LCD display window
								yEnd:the endning y coordinate of the LCD display window
 * @retvalue   :None
******************************************************************************/ 
void LCD180_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	LCD180_WR_REG(lcd180dev.setxcmd);	
	LCD180_WR_DATA(0x00);
	LCD180_WR_DATA(xStar);		
	LCD180_WR_DATA(0x00);
	LCD180_WR_DATA(xEnd);

	LCD180_WR_REG(lcd180dev.setycmd);	
	LCD180_WR_DATA(0x00);
	LCD180_WR_DATA(yStar);		
	LCD180_WR_DATA(0x00);
	LCD180_WR_DATA(yEnd);

	LCD180_WriteRAM_Prepare();	//开始写入GRAM			
}   

/*****************************************************************************
 * @name       :void LCD_SetCursor(u16 Xpos, u16 Ypos)
 * @date       :2018-08-09 
 * @function   :Set coordinate value
 * @parameters :Xpos:the  x coordinate of the pixel
								Ypos:the  y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/ 
void LCD180_SetCursor(u16 Xpos, u16 Ypos)
{	  	    			
	LCD180_SetWindows(Xpos,Ypos,Xpos,Ypos);	
} 

/*****************************************************************************
 * @name       :void LCD_direction(u8 direction)
 * @date       :2018-08-09 
 * @function   :Setting the display direction of LCD screen
 * @parameters :direction:0-0 degree
                          1-90 degree
													2-180 degree
													3-270 degree
 * @retvalue   :None
******************************************************************************/ 
void LCD180_direction(u8 direction)
{ 
			lcd180dev.setxcmd=0x2A;
			lcd180dev.setycmd=0x2B;
			lcd180dev.wramcmd=0x2C;
	switch(direction){		  
		case 0:						 	 		
			lcd180dev.width=LCD180_W;
			lcd180dev.height=LCD180_H;		
			LCD180_WriteReg(0x36,(0<<3)|(1<<6)|(1<<7));//BGR==1,MY==0,MX==0,MV==0
		break;
		case 1:
			lcd180dev.width=LCD180_H;
			lcd180dev.height=LCD180_W;
			LCD180_WriteReg(0x36,(0<<3)|(1<<7)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
		break;
		case 2:						 	 		
			lcd180dev.width=LCD180_W;
			lcd180dev.height=LCD180_H;	
			LCD180_WriteReg(0x36,(0<<3)|(0<<6)|(0<<7));//BGR==1,MY==0,MX==0,MV==0
		break;
		case 3:
			lcd180dev.width=LCD180_H;
			lcd180dev.height=LCD180_W;
			LCD180_WriteReg(0x36,(0<<3)|(0<<7)|(1<<6)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
		break;	
		default:break;
	}		
}	 
