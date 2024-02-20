#include "led.h"
#include "string.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "usart2.h"
//RTOS头文件
#include "FreeRTOS.h"
#include "task.h" 	
#include "limits.h"
//NET
#include "esp32.h"
#include "onenet.h"
#include "uart3.h"
 
 
#define START_TASK_PRIO        1                    //任务优先级
#define START_STK_SIZE         256              //任务栈大小    
TaskHandle_t StartTask_Handler;            //任务句柄
void start_task(void *pvParameters);//任务函数

#define FRMAIN_TASK_PRIO        3                    //任务优先级
#define FRMAIN_STK_SIZE         256              //任务栈大小    
TaskHandle_t FRMAINTask_Handler;            //任务句柄
void FRMAIN_task(void *pvParameters);    //任务函数

#define press_FR_TASK_PRIO        3                    //任务优先级
#define press_FR_STK_SIZE         256              //任务栈大小    
TaskHandle_t press_FRTask_Handler;            //任务句柄
void press_FR_task(void *pvParameters);    //任务函数

#define FR_Fun_TASK_PRIO        2                    //任务优先级
#define FR_Fun_STK_SIZE         256              //任务栈大小    
TaskHandle_t FR_FunTask_Handler;            //任务句柄
void FR_Fun_task(void *pvParameters);    //任务函数

//void Usart1_Sendata();
void Face(void); //人脸识别
//u8 send_buff[] = {"Y\r\n"};

void BoardInit(void)
{
  delay_init();  	    //初始化延时函数
	uart_init(115200);	//初始化串口1波特率为115200，用于支持USMART
	uart2_init(115200);	//初始化串口1波特率为115200，用于支持USMART
	uart3_init();
	LED_Init();
	KEY_Init();
	ESP32_Init();
}

int main(void)
{    
	 BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
	//初始化函数
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	
   
    BoardInit();
    printf("Welcome to FreeRTOS,CoreClock:%d\r\n",SystemCoreClock);
     
    //创建开始任务  使用的函数 xTaskCreate()创建任务(动态方法)
 //的话那么任务堆栈就会由函数 xTaskCreate()自动创建
    xReturn = xTaskCreate((TaskFunction_t )start_task,            //任务函数       
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    if(pdPASS == xReturn)
    vTaskStartScheduler();   //开启任务调度 
  else
    return -1;                                                     
}

void start_task(void *pvParameters)
{
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
    taskENTER_CRITICAL();           //进入临界区
    //创建FRMAIN任务指纹主任务
		xReturn =xTaskCreate((TaskFunction_t )FRMAIN_task,         
												(const char*    )"FRMAIN_task",       
												(uint16_t       )FRMAIN_STK_SIZE, 
												(void*          )NULL,                
												(UBaseType_t    )FRMAIN_TASK_PRIO,    
												(TaskHandle_t*  )&FRMAINTask_Handler);   
		if(pdPASS == xReturn)
    printf("创建FRMAIN任务成功!\r\n");
//								
//		//创建press_FR任务刷指纹						
		xReturn =xTaskCreate((TaskFunction_t )press_FR_task,         
                (const char*    )"press_FR_task",       
                (uint16_t       )press_FR_STK_SIZE, 
                (void*          )NULL,                
                (UBaseType_t    )press_FR_TASK_PRIO,    
                (TaskHandle_t*  )&press_FRTask_Handler); 
    if(pdPASS == xReturn)
    printf("创建press_FR任务成功!\r\n");
		
		xReturn =xTaskCreate((TaskFunction_t )FR_Fun_task,         
                (const char*    )"FR_Fun_task",       
                (uint16_t       )FR_Fun_STK_SIZE, 
                (void*          )NULL,                
                (UBaseType_t    )FR_Fun_TASK_PRIO,    
                (TaskHandle_t*  )&FR_FunTask_Handler); 
		if(pdPASS == xReturn)
    printf("创建FR_Fun任务成功!\r\n");	
		
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

void FRMAIN_task(void *pvParameters){
	u8 key=0;
	u8 LED1_Status;
	u8 LED0_Status;
	u8 FAN0_Status;
	u8 FAN1_Status;
	LED1=1;
	LED0=1;
	FAN0=1;
	FAN1=0;
	while(1){
		key=KEY_Scan(0);
		switch(key){
			case KEY0_PRES:
			//LED0=1;
			LED0=!LED0;
			LED0_Status=get_LED0_status();
			break;
			
			case KEY1_PRES:
			//LED1=1;
			LED1=!LED1;
			LED1_Status=get_LED1_status();
			break;
			
			case KEY2_PRES:
			//LED1=1;
			FAN0=!FAN0;
			FAN0_Status=get_FAN0_status();
			break;
			
			case KEY3_PRES:
			//LED1=1;
			FAN1=!FAN1;
			FAN1_Status=get_FAN1_status();
			break;
			
		  case WKUP_PRES:
			LED1=!LED1;
			LED0=!LED0;
			LED1_Status=get_LED1_status();
			LED0_Status=get_LED0_status();
			break;
			
			default:
			delay_ms(10);	
		}
	}
}

void press_FR_task(void *pvParameters){
	while(1){
		Face();
		vTaskDelay(500);
	}
}

void FR_Fun_task(void *pvParameters){
	uint16_t timeCount = 0;
  uint8_t *dataPtr = NULL;
//连接OneNet——连接不成功则不跳出
  while(MqttLinkOneNet())
    delay_ms(500);
  while(1) {
  //发送间隔5s	
    if(++timeCount >= 500) {
      printf("OneNet_SendData\r\n");
      MqttSendDataOneNet(); //发送数据
			
      timeCount = 0;
      uart3_clear();
    }
		
    dataPtr = ESP32_GetIPD(0);
    if(dataPtr != NULL)
      MqttRecvProOneNet(dataPtr);    
    delay_ms(10);
  }
}

//void Usart1_Sendata()
//{	
//	u8 i = 0;
//	
////	USART_SendData(USART1,0x0d);
////	while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);
//	
//		USART_SendData(USART1,send_buff[0]);
//		while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);
//	
//}

 //人脸识别
void Face(void){
	if(USART2_RX_STA){
		printf("USART2_RX_BUF:%s\r\n",USART2_RX_BUF);
	if(USART2_RX_BUF[0]==/*send_buff[0]*/0x59) //ascii码表的值 16进制 0x31==十进制的1 
		{
			USART_SendData(USART2,0x59/*USART2_RX_BUF[0]*/);
			while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);
			memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF));
			count=0;
			USART2_RX_STA=0;
		}
		else
		{
			memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF));
			count=0;
			USART2_RX_STA=0;
			printf("face fail\r\n");
		}
	}
}
