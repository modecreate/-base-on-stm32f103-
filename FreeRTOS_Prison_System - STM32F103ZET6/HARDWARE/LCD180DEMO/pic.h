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
#ifndef __PIC_H
#define __PIC_H 

//16位BMP 40X40 QQ图像取模数据
//Image2LCD取模选项设置
//水平扫描
//16位
//40X40
//不包含图像头数据
//自左至右
//自顶至底
//低位在前
const unsigned char gImage_qq[3200] = { /* 0X00,0X10,0X28,0X00,0X28,0X00,0X01,0X1B, */
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBE,0XF7,
0XFF,0XFF,0XDE,0XFF,0X38,0XC6,0X92,0X8C,0X8E,0X6B,0X6E,0X6B,0X10,0X7C,0X96,0XAD,
0X3C,0XE7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5D,0XEF,
0X15,0X9D,0X4F,0X63,0X6C,0X42,0X0A,0X32,0X88,0X29,0X46,0X19,0X25,0X19,0X45,0X21,
0XE8,0X31,0X8E,0X6B,0X38,0XC6,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X36,0XA5,0X10,0X53,
0X10,0X4B,0X51,0X53,0X0F,0X4B,0X6C,0X3A,0XE9,0X31,0X67,0X21,0X25,0X19,0XE4,0X10,
0XA3,0X08,0X62,0X00,0X83,0X08,0XCB,0X52,0X9A,0XD6,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X3C,0XE7,0X70,0X63,0XB3,0X63,0XB8,0X7C,
0XF5,0X63,0X11,0X43,0X4D,0X32,0XEA,0X29,0X88,0X21,0X26,0X19,0X05,0X19,0X05,0X19,
0X04,0X11,0X04,0X11,0XE4,0X10,0X83,0X00,0XA3,0X08,0X72,0X8C,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDB,0XDE,0X4B,0X3A,0XF0,0X42,0X35,0X6C,0X54,0X4B,
0XB1,0X32,0X2E,0X2A,0XEB,0X21,0XA9,0X21,0X67,0X19,0X05,0X19,0X04,0X11,0X04,0X11,
0X04,0X11,0X04,0X11,0X04,0X11,0X05,0X19,0XE4,0X10,0X42,0X00,0XAF,0X73,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0X5D,0XEF,0X09,0X32,0X4C,0X32,0X10,0X4B,0X8F,0X32,0X4F,0X2A,
0X2E,0X2A,0XCC,0X19,0X89,0X19,0X89,0X21,0X47,0X19,0X05,0X19,0X04,0X11,0X04,0X11,
0XC4,0X10,0XC4,0X10,0X04,0X11,0X04,0X11,0X04,0X11,0XE4,0X10,0X42,0X00,0X31,0X84,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XEC,0X52,0X47,0X19,0X4C,0X32,0X0B,0X2A,0XEC,0X21,0XEC,0X21,
0X0C,0X22,0X91,0X5B,0XEE,0X4A,0X06,0X11,0X26,0X19,0X04,0X19,0XE4,0X10,0XE4,0X10,
0XA7,0X29,0X66,0X21,0XA3,0X08,0X05,0X19,0X04,0X11,0X04,0X11,0XE4,0X10,0X82,0X00,
0XF7,0XBD,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0X35,0XA5,0X83,0X08,0X88,0X21,0X88,0X21,0X89,0X21,0XAA,0X21,0X8A,0X21,
0X6B,0X42,0X71,0X8C,0XFF,0XFF,0X72,0X8C,0X83,0X08,0X04,0X11,0XC4,0X08,0X29,0X42,
0XFB,0XDE,0X5D,0XEF,0XEC,0X5A,0X83,0X08,0X04,0X11,0X04,0X11,0X04,0X11,0X83,0X08,
0XE8,0X31,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XBE,0XF7,0XC7,0X31,0XC4,0X10,0X25,0X19,0X26,0X19,0X47,0X19,0X47,0X19,0XA8,0X29,
0X8A,0X52,0X28,0X4A,0X55,0XAD,0XFF,0XFF,0XE8,0X31,0XA3,0X08,0X05,0X19,0X4D,0X6B,
0X4D,0X6B,0XFF,0XFF,0X7D,0XEF,0X45,0X21,0XC4,0X10,0X04,0X11,0X04,0X11,0X04,0X11,
0X62,0X00,0X76,0XAD,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X96,0XB5,0X62,0X00,0X04,0X11,0X04,0X19,0X05,0X11,0X05,0X19,0XC4,0X08,0X8B,0X4A,
0XB6,0XB5,0X5D,0XEF,0XF7,0XBD,0XFF,0XFF,0X8E,0X6B,0X62,0X00,0X29,0X42,0XAA,0X5A,
0X08,0X42,0XFF,0XFF,0XFF,0XFF,0XCC,0X52,0X83,0X08,0X04,0X11,0X04,0X11,0X04,0X11,
0XA3,0X08,0XAD,0X52,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X1C,0XE7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X4E,0X63,0X62,0X00,0X04,0X11,0X04,0X11,0X04,0X11,0XE4,0X10,0X62,0X00,0X8E,0X63,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XCF,0X73,0X01,0X00,0XF3,0X9C,0X2C,0X63,
0X96,0XB5,0XFF,0XFF,0XFF,0XFF,0X2D,0X5B,0X83,0X00,0X04,0X11,0X04,0X11,0X04,0X11,
0XE4,0X10,0X67,0X21,0X3D,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XF8,0XBD,0XB7,0XB5,0X9E,0XEF,0XCB,0X52,0XB3,0X94,0XFF,0XFF,0XFF,0XFF,0XDF,0XFF,
0XE8,0X31,0XA3,0X08,0X04,0X11,0X04,0X11,0X04,0X11,0X04,0X11,0XA3,0X08,0X49,0X42,
0XFF,0XF7,0XFF,0XF7,0XFF,0XFF,0XFF,0XFF,0X6A,0X4A,0X01,0X00,0X72,0X84,0XFF,0XFF,
0XFF,0XFF,0XFF,0XF7,0XDF,0XEF,0X09,0X3A,0XA3,0X08,0X04,0X11,0X04,0X11,0X04,0X11,
0X04,0X11,0X05,0X11,0X18,0XBE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XF0,0X7B,0X62,0X00,0XE8,0X31,0XC7,0X31,0X41,0X00,0X35,0XA5,0XFF,0XFF,0X5D,0XEF,
0X46,0X21,0XC4,0X10,0X04,0X11,0X04,0X11,0X04,0X11,0X04,0X11,0XE4,0X10,0XA3,0X08,
0X76,0X9D,0XFF,0XF7,0XFF,0XFF,0XB7,0XAD,0XA3,0X08,0XA3,0X08,0XC7,0X31,0X9E,0XE7,
0XFF,0XF7,0XFF,0XF7,0X76,0XA5,0XA3,0X08,0XE4,0X10,0X04,0X11,0X04,0X11,0X04,0X11,
0X05,0X11,0X05,0X11,0X35,0XA5,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XDB,0XDE,0XA7,0X29,0X83,0X00,0XC4,0X10,0XC4,0X10,0XC4,0X10,0X1C,0XE7,0X9E,0XEF,
0X05,0X11,0XE4,0X10,0X04,0X11,0X04,0X11,0X04,0X11,0X04,0X11,0X04,0X19,0XC4,0X08,
0XE5,0X10,0XD1,0X6B,0XD1,0X6B,0XC5,0X08,0X64,0X00,0XA5,0X08,0X43,0X00,0X2B,0X32,
0X77,0X9D,0XB3,0X84,0X25,0X19,0XC4,0X10,0X04,0X11,0X04,0X11,0X04,0X11,0X04,0X11,
0X25,0X19,0X26,0X09,0X35,0X9D,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XAF,0X73,0X62,0X00,0X04,0X19,0X05,0X19,0X82,0X00,0X0D,0X5B,0X8E,0X9B,
0X62,0X10,0X05,0X11,0X04,0X11,0X04,0X11,0X04,0X19,0XE4,0X10,0X85,0X00,0X05,0X11,
0XC4,0X39,0X81,0X5A,0X40,0X7B,0X22,0X9C,0X43,0XAC,0X03,0XA4,0X83,0X9B,0X82,0X72,
0X82,0X49,0XC2,0X18,0XA4,0X00,0XC5,0X00,0XE4,0X10,0X04,0X19,0X04,0X11,0X05,0X19,
0X47,0X19,0X67,0X11,0XEC,0X5A,0XBE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XDF,0XFF,0XDB,0XDE,0XC4,0X10,0XE4,0X10,0X04,0X11,0X05,0X11,0XA4,0X18,0X01,0XC0,
0X83,0X88,0XE4,0X00,0X05,0X19,0X04,0X19,0XC5,0X08,0X44,0X21,0X43,0X83,0X23,0XD5,
0X42,0XFE,0XE4,0XFE,0X27,0XFF,0X07,0XFF,0XA4,0XFE,0X64,0XFE,0X03,0XFE,0XA3,0XFD,
0XE2,0XFC,0X42,0XEC,0X83,0XB3,0X24,0X62,0XE5,0X10,0XC4,0X08,0X04,0X19,0X26,0X19,
0XA8,0X19,0X87,0X21,0X00,0X90,0XD3,0XBC,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XDF,0XFF,0XFF,0XFF,0X10,0X7C,0X42,0X00,0X05,0X19,0X05,0X11,0X83,0X28,0X01,0XD0,
0X44,0XF8,0XA3,0X48,0XE4,0X00,0XC5,0X08,0X44,0X5A,0X02,0XED,0XE2,0XFD,0X02,0XFE,
0X66,0XFE,0X74,0XFF,0XB8,0XFF,0X73,0XFF,0XE7,0XF6,0XA6,0XF6,0X45,0XF6,0XA4,0XF5,
0XC3,0XFC,0X62,0XFC,0XC2,0XFC,0XC2,0XFC,0XE3,0XCB,0XC4,0X49,0X06,0X11,0X88,0X19,
0X87,0X01,0XA4,0X90,0X01,0XF8,0XEC,0X9A,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XBE,0XF7,0XE8,0X31,0X83,0X00,0X05,0X09,0X82,0X40,0X01,0XC0,
0X23,0XF8,0X85,0XF0,0XA3,0X48,0XA4,0X00,0X44,0X5A,0X02,0XFD,0X23,0XCC,0XC2,0XDC,
0X04,0XFE,0X28,0XFE,0X48,0XF6,0X46,0XF6,0X24,0XF6,0XE4,0XF5,0X64,0XFD,0XE3,0XFC,
0X62,0XFC,0XC2,0XFC,0X02,0XE4,0X02,0XDC,0XE2,0XFC,0XA4,0X7A,0X48,0X01,0X67,0X01,
0XC4,0X78,0X24,0XF8,0X02,0XF8,0X84,0XB0,0X7D,0XE7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDB,0XDE,0X25,0X19,0XA3,0X00,0XC4,0X38,0X02,0XE0,
0X22,0XD8,0X44,0XF8,0XA6,0XF8,0XA4,0X78,0X63,0X00,0X43,0X21,0X83,0X72,0X83,0X39,
0X82,0X9B,0X21,0XF5,0X61,0XFD,0X22,0XFD,0XE2,0XFC,0XA2,0XFC,0X42,0XFC,0X42,0XFC,
0X42,0XFC,0X22,0XAB,0X83,0X41,0XC3,0X92,0X04,0X52,0X26,0X01,0X25,0X19,0XA4,0X98,
0X44,0XF8,0X23,0XF8,0X02,0XF8,0XA4,0XD0,0X9E,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X9A,0XD6,0X87,0X29,0XA5,0X00,0X43,0XB8,
0X22,0XF8,0X23,0XE0,0X65,0XF8,0XE8,0XF8,0X07,0XC9,0X83,0X48,0X42,0X00,0XA3,0X00,
0X84,0X00,0X63,0X29,0XA2,0X7A,0X62,0XB3,0XA2,0XCB,0X62,0XD3,0X02,0XBB,0X82,0X8A,
0X83,0X39,0XA4,0X00,0XE5,0X00,0XE5,0X00,0XE5,0X08,0XC4,0X60,0X64,0XD8,0X44,0XF8,
0X24,0XF8,0X23,0XF8,0X02,0XF8,0X83,0X88,0XDB,0XC6,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X3D,0XE7,0X50,0X5B,0X08,0X31,
0X23,0XE8,0X43,0XF8,0X44,0XF0,0X65,0XF8,0X09,0XF9,0XAB,0XF9,0X89,0XD1,0X06,0X89,
0XA3,0X48,0X42,0X18,0X02,0X00,0X42,0X00,0X61,0X00,0X82,0X00,0X62,0X00,0X62,0X00,
0X83,0X00,0XA3,0X20,0XC4,0X50,0XA5,0X88,0X85,0XD8,0X65,0XF8,0X44,0XF8,0X44,0XF8,
0X23,0XF8,0X23,0XF8,0X03,0XD0,0X82,0X10,0XC7,0X29,0X5D,0XEF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X6C,0X32,
0XA5,0X38,0X02,0XD8,0X23,0XF8,0X65,0XF8,0X66,0XF8,0XA7,0XF8,0X4A,0XF9,0X0C,0XFA,
0X4D,0XFA,0X4C,0XEA,0X0B,0XD2,0XA9,0XB9,0X68,0XB1,0X47,0XA9,0X27,0XB1,0X07,0XB9,
0X07,0XD1,0XE7,0XE8,0XC7,0XF8,0XA7,0XF8,0X65,0XF8,0X65,0XF8,0X44,0XF8,0X23,0XF8,
0X03,0XF8,0X02,0XD0,0XA3,0X28,0X05,0X09,0XC4,0X08,0XEC,0X5A,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFB,0XDE,0X05,0X19,
0XC4,0X00,0XA7,0X41,0XE6,0XC0,0X03,0XF8,0X86,0XF8,0XA7,0XF8,0X87,0XF8,0X86,0XF8,
0XC7,0XF8,0X29,0XF9,0X8A,0XF9,0XAB,0XF9,0XAB,0XF9,0X8B,0XF9,0X6A,0XF9,0X29,0XF9,
0X08,0XF9,0XC7,0XF8,0XA6,0XF8,0X86,0XF8,0X65,0XF8,0X64,0XF8,0X23,0XF8,0X02,0XF0,
0X06,0XB1,0X25,0X29,0XE4,0X00,0XE4,0X10,0X25,0X19,0X25,0X19,0X14,0X9D,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X96,0XAD,0X62,0X00,
0X82,0X08,0X35,0X95,0XBA,0XCE,0X8B,0XA2,0X44,0XD0,0X25,0XF8,0X87,0XF8,0XA7,0XF8,
0XC7,0XF8,0XA7,0XF8,0X87,0XF8,0X86,0XF8,0X86,0XF8,0X86,0XF8,0X87,0XF8,0XA7,0XF8,
0XA7,0XF8,0XA6,0XF8,0X85,0XF8,0X65,0XF8,0X64,0XF8,0X24,0XF0,0X64,0XB8,0X0D,0X93,
0XBB,0XB6,0XCF,0X63,0X83,0X08,0X04,0X11,0XE4,0X10,0X66,0X21,0X49,0X3A,0X5D,0XEF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XD3,0X94,0X42,0X00,
0XE4,0X10,0XBB,0XCE,0XFF,0XFF,0XBE,0XE7,0X76,0XB5,0XCC,0XAA,0X07,0XC1,0X45,0XE0,
0X45,0XF8,0X46,0XF8,0X66,0XF8,0X86,0XF8,0X86,0XF8,0X86,0XF8,0X86,0XF8,0X65,0XF8,
0X45,0XF8,0X65,0XF8,0X65,0XE8,0X44,0XD0,0X43,0XA8,0X01,0X88,0X82,0X90,0X3C,0XD7,
0XFF,0XEF,0X55,0X95,0X83,0X08,0X04,0X11,0X04,0X11,0X05,0X19,0X46,0X19,0XB3,0X94,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XB3,0X94,0X41,0X00,
0X86,0X21,0X5D,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDF,0XE7,0X7A,0XC6,0XD3,0XB4,
0X4E,0XB3,0X2A,0XC2,0X68,0XD1,0XE6,0XE0,0XA6,0XE8,0XA5,0XE8,0XA5,0XE8,0XE6,0XD8,
0X88,0XC9,0X06,0XA9,0X22,0XA8,0X02,0XA8,0X00,0XA0,0X00,0XC8,0X00,0XD8,0XF7,0XE5,
0XFF,0XE7,0XF8,0XAD,0XC4,0X10,0XE4,0X10,0X04,0X11,0XE4,0X10,0X05,0X11,0X8B,0X4A,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X55,0XA5,0X41,0X00,
0XA7,0X29,0X5D,0XDF,0XFF,0XF7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XEF,0X7D,0XDF,0XDB,0XCE,0X59,0XCE,0XF8,0XCD,0XD7,0XCD,0XF7,0XC5,0X79,0XCE,
0XFB,0XBE,0XAB,0XA2,0X03,0XF0,0X45,0XF8,0X42,0XD0,0X43,0XE8,0X00,0XF0,0X72,0XD4,
0XFF,0XDF,0X39,0XAE,0XE4,0X10,0XE4,0X10,0X04,0X11,0XE4,0X10,0X05,0X11,0X87,0X29,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X59,0XCE,0X83,0X08,
0X46,0X21,0X1C,0XD7,0XFF,0XF7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XEF,0X8B,0XBA,0X04,0XF8,0X45,0XF8,0X62,0XE0,0X44,0XF0,0X00,0XF8,0X8E,0XDB,
0XFF,0XDF,0XF8,0XA5,0XC4,0X10,0XE4,0X10,0XE4,0X10,0X04,0X11,0XE4,0X10,0X25,0X19,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBE,0XF7,0X87,0X29,
0X83,0X08,0X39,0XB6,0XFF,0XF7,0XDF,0XF7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XBE,0XE7,0X4A,0XBA,0X03,0XF8,0X45,0XF8,0X64,0XF8,0X44,0XF8,0X00,0XF8,0X6E,0XE3,
0XFF,0XD7,0XF4,0X8C,0X83,0X08,0X04,0X11,0XE4,0X10,0XE4,0X10,0XE4,0X10,0X05,0X19,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEF,0X73,
0X00,0X00,0X72,0X84,0XFF,0XEF,0XBE,0XEF,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XDF,0XE7,0X8B,0XBA,0X03,0XF8,0X45,0XF8,0X45,0XF8,0X23,0XF8,0X00,0XF8,0XD3,0XD4,
0XFF,0XD7,0X4E,0X5B,0X21,0X00,0X29,0X3A,0X55,0XA5,0X83,0X08,0XC4,0X10,0X25,0X19,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFB,0XDE,
0XA3,0X08,0XE8,0X31,0X9E,0XDF,0X9E,0XE7,0XBF,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF7,0X51,0XBC,0X02,0XE0,0X03,0XF8,0X03,0XF0,0X43,0XE0,0XEC,0XC2,0X7E,0XCF,
0XFC,0XBE,0X46,0X21,0X21,0X00,0XD3,0X94,0XFF,0XFF,0X51,0X84,0X00,0X00,0X87,0X29,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X51,0X84,0X00,0X00,0XF4,0X8C,0XFF,0XEF,0X9E,0XE7,0XBF,0XEF,0XDF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0X3D,0XDF,0X55,0XBD,0X52,0XBC,0X72,0XBC,0XB7,0XB5,0X5D,0XC7,0XFF,0XDF,
0XF0,0X6B,0X00,0X00,0X09,0X3A,0XBF,0XF7,0XFF,0XFF,0XFF,0XFF,0X14,0X9D,0X55,0XA5,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XAC,0X4A,0XA4,0X08,0XBB,0XBE,0XDF,0XE7,0X7E,0XE7,0XBE,0XEF,0XDF,0XF7,
0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XDF,0XFF,0XDF,0XF7,0XFF,0XEF,0XDF,0XDF,0XBF,0XD7,0X9E,0XD7,0XDF,0XDF,0XD8,0XA5,
0X83,0X08,0X26,0X11,0XDB,0XD6,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X79,0XEE,0X8B,0XDC,0X21,0X31,0XA9,0X21,0X3D,0XCF,0XBF,0XDF,0X7E,0XDF,0X9E,0XE7,
0XBE,0XEF,0XBF,0XEF,0XDF,0XF7,0XDF,0XF7,0XDF,0XF7,0XDF,0XF7,0XDF,0XF7,0XBF,0XEF,
0XBE,0XEF,0X9E,0XE7,0X7E,0XDF,0X5E,0XD7,0X5E,0XD7,0XDF,0XDF,0X9A,0XB6,0X26,0X19,
0X42,0X08,0XED,0XA3,0XBF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X74,0XDD,
0XC0,0XDB,0X00,0XFE,0X42,0XEE,0X02,0X42,0X89,0X21,0X7B,0XB6,0XDF,0XDF,0X7E,0XD7,
0X7E,0XDF,0X7E,0XDF,0X9E,0XE7,0X9E,0XE7,0X9E,0XE7,0X9E,0XE7,0X9E,0XE7,0X7E,0XDF,
0X7E,0XDF,0X5D,0XD7,0X5D,0XD7,0X9E,0XDF,0XFF,0XE7,0XF8,0XA5,0X07,0X11,0XE3,0X18,
0X02,0XC5,0X60,0XFD,0XE6,0XD3,0XDB,0XEE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X9E,0XF7,0X84,0XBA,
0XC1,0XFC,0X42,0XFE,0X82,0XFE,0XA2,0XFE,0X81,0X83,0X45,0X21,0X74,0X74,0X5E,0XC7,
0XDF,0XDF,0X7E,0XD7,0X5E,0XD7,0X5D,0XD7,0X5E,0XD7,0X5E,0XD7,0X5D,0XD7,0X5D,0XD7,
0X5E,0XD7,0X9E,0XDF,0XFF,0XE7,0X3D,0XC7,0XF1,0X63,0X84,0X08,0X42,0X52,0X26,0XE6,
0X29,0XFF,0X86,0XFE,0XE0,0XF3,0X6A,0XC3,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X18,0XDE,0XC1,0XD2,
0XA2,0XFD,0X22,0XFE,0X42,0XFE,0X42,0XFE,0X62,0XFE,0XE2,0XD4,0X41,0X6A,0X49,0X42,
0X53,0X74,0X3B,0XA6,0X3E,0XC7,0XBF,0XD7,0XBF,0XD7,0XBF,0XD7,0XBF,0XDF,0XBF,0XD7,
0X3E,0XC7,0X1A,0XA6,0XF2,0X63,0XA7,0X29,0X82,0X41,0X22,0XB4,0X62,0XFE,0X83,0XFE,
0XAA,0XFE,0X0F,0XFF,0X67,0XFD,0X63,0XBA,0X3C,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X9A,0XE6,0X80,0XD2,
0X21,0XFD,0XC2,0XFD,0XE2,0XF5,0XC2,0XF5,0X82,0XF5,0X82,0XFD,0X62,0XFD,0X61,0XDC,
0X21,0X9B,0X84,0X6A,0XE9,0X6A,0X2C,0X63,0XAF,0X63,0X11,0X74,0X6E,0X63,0X2C,0X63,
0X89,0X5A,0X04,0X52,0X81,0X7A,0XC2,0XCB,0XE2,0XFC,0X62,0XFD,0X82,0XFD,0XC2,0XFD,
0XC2,0XFD,0XE4,0XFD,0X24,0XFD,0X62,0XCA,0X1C,0XE7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XB1,0XCC,
0X81,0XD2,0XC0,0XF3,0XC1,0XFC,0X02,0XFD,0X02,0XFD,0XE2,0XFC,0XC2,0XFC,0XC2,0XFC,
0X81,0XFC,0X80,0XFB,0XC0,0XC9,0XA4,0X81,0X35,0XAD,0X59,0XCE,0X71,0X9C,0X21,0X81,
0X00,0XDA,0XA1,0XFB,0X82,0XFC,0XA2,0XFC,0X82,0XFC,0XA2,0XFC,0X02,0XFD,0X22,0XFD,
0XE2,0XFC,0X00,0XFC,0X60,0XDA,0X90,0XCC,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X59,0XDE,0X0D,0XC4,0X06,0XCB,0XE4,0XD2,0X03,0XDB,0X03,0XDB,0XE3,0XDA,0XC3,0XD2,
0XA4,0XC2,0X09,0XB3,0XD2,0XBC,0X9E,0XF7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFB,0XE6,
0X0E,0XB4,0XA6,0XBA,0X83,0XD2,0XE3,0XE2,0X02,0XEB,0X22,0XEB,0X22,0XE3,0X03,0XDB,
0XE4,0XD2,0X6A,0XC3,0XB6,0XD5,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
};
#endif
