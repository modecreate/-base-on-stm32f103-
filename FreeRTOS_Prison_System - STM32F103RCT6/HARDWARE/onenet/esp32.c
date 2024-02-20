#include <string.h>
#include <stdio.h>
#include "stm32f10x.h"
#include "esp32.h"
#include "delay.h"
#include "usart.h"
#include "uart3.h"


#define delay_ms delay_ms

#define ESP32_WIFI_INFO		"AT+CWJAP=\"12345\",\"12345678\"\r\n"

#define ESP32_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"183.230.40.39\",6002\r\n"


//==========================================================
//	函数名称：	ESP32_Clear
//
//	函数功能：	清空缓存
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void ESP32_Clear(void)
{
	uart3_clear();
}

//==========================================================
//	函数名称：	ESP32_WaitRecive
//
//	函数功能：	等待接收完成
//
//	入口参数：	无
//
//	返回参数：	REV_OK-接收完成		REV_WAIT-接收超时未完成
//
//	说明：		循环调用检测是否接收完成
//==========================================================
_Bool ESP32_WaitRecive(void)
{

	if(wifi_dev.flag == 0) 							//如果接收计数为0 则说明没有处于接收数据中，所以直接跳出，结束函数
		return REV_WAIT;
	if(wifi_dev.flag)
		return REV_OK;
}

//==========================================================
//	函数名称：	ESP32_SendCmd
//
//	函数功能：	发送命令
//
//	入口参数：	cmd：命令
//				res：需要检查的返回指令
//
//	返回参数：	0-成功	1-失败
//
//	说明：		
//==========================================================
_Bool ESP32_SendCmd(char *cmd, char *res)
{
	
	unsigned char timeOut = 200;

	USART3_SendString((uint8_t*)cmd,strlen(cmd));
	
	while(timeOut--)
	{
		if(ESP32_WaitRecive() == REV_OK)							//如果收到数据
		{
			if(strstr((const char *)wifi_dev.esp_buf, res) != NULL)		//如果检索到关键词
			{
				ESP32_Clear();									//清空缓存
				printf("cmd:%s  res:%s\r\n",cmd,res);
				return 0;
			}
		}
		ESP32_Clear();
		delay_ms(10);
	}
	
	return 1;

}

//==========================================================
//	函数名称：	ESP32_SendData
//
//	函数功能：	发送数据
//
//	入口参数：	data：数据
//				len：长度
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void ESP32_SendData(unsigned char *data, unsigned short len)
{

	char cmdBuf[32];
	
	ESP32_Clear();								//清空接收缓存
	sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);		//发送命令
	if(!ESP32_SendCmd(cmdBuf, ">"))				//收到‘>’时可以发送数据
	{
		USART3_SendString((uint8_t*)data,len);		//发送设备连接请求数据
	}

}

//==========================================================
//	函数名称：	ESP32_GetIPD
//
//	函数功能：	获取平台返回的数据
//
//	入口参数：	等待的时间(乘以10ms)
//
//	返回参数：	平台返回的原始数据
//
//	说明：		不同网络设备返回的格式不同，需要去调试
//				如ESP32的返回格式为	"+IPD,x:yyy"	x代表数据长度，yyy是数据内容
//==========================================================
unsigned char *ESP32_GetIPD(unsigned short timeOut)
{

	char *ptrIPD = NULL;
	
	do
	{
		if(ESP32_WaitRecive() == REV_OK)								//如果接收完成
		{
			ptrIPD = strstr((char *)wifi_dev.esp_buf, "IPD,");				//搜索“IPD”头
			if(ptrIPD == NULL)											//如果没找到，可能是IPD头的延迟，还是需要等待一会，但不会超过设定的时间
			{
				//UsartPrintf(USART_DEBUG, "\"IPD\" not found\r\n");
			}
			else
			{
				ptrIPD = strchr(ptrIPD, ':');							//找到':'
				if(ptrIPD != NULL)
				{
					ptrIPD++;
					return (unsigned char *)(ptrIPD);
				}
				else
					return NULL;
				
			}
		}
		
		delay_ms(5);													//延时等待
	} while(timeOut--);
	
	return NULL;														//超时还未找到，返回空指针

}

//==========================================================
//	函数名称：	ESP32_Init
//
//	函数功能：	初始化ESP32
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void ESP32_Init(void)
{
	
	GPIO_InitTypeDef GPIO_Initure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	//ESP32复位引脚
	GPIO_Initure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Initure.GPIO_Pin = GPIO_Pin_4;					//GPIOC14-复位
	GPIO_Initure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_Initure);
	
	GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_RESET);
	delay_ms(250);
	GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_SET);
	delay_ms(500);
	
	ESP32_Clear();
	
	while(ESP32_SendCmd("AT\r\n", "OK"));
		delay_ms(500);
	
	while(ESP32_SendCmd("AT+CWMODE=1\r\n", "OK"));
		delay_ms(500);
	
	while(ESP32_SendCmd(ESP32_WIFI_INFO, "GOT IP"));
		delay_ms(500);
	
	while(ESP32_SendCmd(ESP32_ONENET_INFO, "CONNECT"));
		delay_ms(500);

}
