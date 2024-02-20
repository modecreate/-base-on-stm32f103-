#include "sys.h"
#include "string.h"
#include "math.h"	
#include "delay.h"
#include "usart.h"
#include "usart3.h"
#include "usmart.h" 
#include "key.h"  
#include "malloc.h"  
#include "usart2.h"
#include "AS608.h"
#include "timer.h"
#include "math.h"	
#include "adc.h"
//#include "led.h"
//舵机蜂鸣器
#include "pwm.h"
#include "beep.h"
//LCD180
#include "lcd180.h"
#include "lcd.h"
#include "gui.h"
//DHT11
#include "dht11.h"
//RTOS头文件
#include "FreeRTOS.h"
#include "task.h" 	
#include "limits.h"
//ov7725
#include "ov7725.h"
#include "sccb.h"
#include "tpad.h"
#include "exti.h"
//sd
#include "sdio_sdcard.h"  
//FATFS
#include "w25qxx.h"    
#include "ff.h"  
#include "exfuns.h"   
//PITCURE
#include "piclib.h"	
//TEXT
#include "text.h"
//Mini STM32F103开发板实验
//AS608指纹识别模块串口通讯实验-库函数版本   


#define usart2_baund  115200//串口2波特率，根据指纹模块波特率更改（注意：指纹模块默认57600）   错误点  注意：指纹模块默认57600 改成115200即可
SysPara AS608Para;//指纹模块AS608参数
u16 ValidN;//模块内有效模板个数
u8** kbd_tbl;

#define START_TASK_PRIO        1                    //任务优先级
#define START_STK_SIZE         256              //任务栈大小    
TaskHandle_t StartTask_Handler;            //任务句柄
void start_task(void *pvParameters);//任务函数
 
#define FRMAIN_TASK_PRIO        2                    //任务优先级
#define FRMAIN_STK_SIZE         256              //任务栈大小    
TaskHandle_t FRMAINTask_Handler;            //任务句柄
void FRMAIN_task(void *pvParameters);    //任务函数
 
#define FR_Fun_TASK_PRIO        4                    //任务优先级
#define FR_Fun_STK_SIZE         256              //任务栈大小    
TaskHandle_t FR_FunTask_Handler;            //任务句柄
void FR_Fun_task(void *pvParameters);    //任务函数


#define press_FR_TASK_PRIO        3                    //任务优先级
#define press_FR_STK_SIZE         256              //任务栈大小    
TaskHandle_t press_FRTask_Handler;            //任务句柄
void press_FR_task(void *pvParameters);    //任务函数

#define LCD180DHT11_TASK_PRIO        6                    //任务优先级
#define LCD180DHT11_STK_SIZE         256              //任务栈大小    
TaskHandle_t LCD180DHT11Task_Handler;            //任务句柄
void LCD180DHT11_task(void *pvParameters);    //任务函数

#define LCD180MQ2_TASK_PRIO        7                    //任务优先级
#define LCD180MQ2_STK_SIZE         256              //任务栈大小    
TaskHandle_t LCD180MQ2Task_Handler;            //任务句柄
void LCD180MQ2_task(void *pvParameters);    //任务函数

#define FaceDo_TASK_PRIO        5                    //任务优先级
#define FaceDo_STK_SIZE         256              //任务栈大小    
TaskHandle_t FaceDoTask_Handler;            //任务句柄
void FaceDo_task(void *pvParameters);    //任务函数

//#define TEST_TASK_PRIO        9                    //任务优先级
//#define TEST_STK_SIZE         256              //任务栈大小    
//TaskHandle_t TESTTask_Handler;            //任务句柄
//void TEST_task(void *pvParameters);    //任务函数

#define LCD_TASK_PRIO        8                    //任务优先级
#define LCD_STK_SIZE         256              //任务栈大小    
TaskHandle_t LCDTask_Handler;            //任务句柄
void LCD_task(void *pvParameters);    //任务函数

void Add_FR(void);	//录指纹
void Del_FR(void);	//删除指纹
void press_FR(void);//刷指纹
void doSG90(void); //SG90舵机工作
void Face(void); //人脸识别
void Beep_1(void); //警报1次
void Beep_2(void); //警报2次
void Beep_3(void); //警报3次
void ShowErrMessage(u8 ensure);//显示确认码错误信息 


//ov7725
void camera_refresh(void);
void camera_new_pathname(u8 *pname);
#define  OV7725_WINDOW_WIDTH		320 	// <=320
#define  OV7725_WINDOW_HEIGHT		240 	// <=240

const u8*LMODE_TBL[6]={"Auto","Sunny","Cloudy","Office","Home","Night"};//6种光照模式	    
const u8*EFFECTS_TBL[7]={"Normal","Negative","B&W","Redish","Greenish","Bluish","Antique"};	//7种特效 
extern u8 ov_sta;	//在exit.c里 面定义
extern u8 ov_frame;	//在timer.c里面定义	


void BoardInit(void)
{
	
  delay_init();  	    //初始化延时函数
	Adc_Init();//ADC初始化
	SystemInit();	//System init.
	BEEP_Init();  //蜂鸣器初始化
	uart_init(115200);	//初始化串口1波特率为115200，用于支持USMART
	uart3_init(115200);	//初始化串口1波特率为115200，用于支持USMART
	usart2_init(usart2_baund);//初始化串口2,用于与指纹模块通讯
	LCD180_Init();
	LCD_Init();
	DHT11_Init();
	PS_StaGPIO_Init();	//初始化FR读状态引脚
	KEY_Init();					//按键初始化 
	TIM3_PWM_Init(199,7199);	 //(199+1)*(7199+1)/72*10^6
	TIM5_Int_Init(4999,7199);
	mem_init();					//初始化内存池 
	exfuns_init();			//为fatfs相关变量申请内存  
	f_mount(fs[0],"0:",1); 		//挂载SD卡 
 	f_mount(fs[1],"1:",1);  //挂载FLASH.
	
	usmart_dev.init(72);		//初始化USMART	
	//LED_Init();		  			//初始化与LED连接的硬件接口
	TPAD_Init(6);
	W25QXX_Init();				//初始化W25Q128
}

//显示确认码错误信息
void ShowErrMessage(u8 ensure)
{
	//LCD_Fill(0,120,lcddev.width,160,WHITE);
	//Show_Str_Mid(0,120,(u8*)EnsureMessage(ensure),16,240);
	printf("%s",EnsureMessage(ensure));
	delay_xms(10);
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
								
		//创建FR_Fun任务指纹						
		xReturn =xTaskCreate((TaskFunction_t )FR_Fun_task,         
                (const char*    )"FR_Fun_task",       
                (uint16_t       )FR_Fun_STK_SIZE, 
                (void*          )NULL,                
                (UBaseType_t    )FR_Fun_TASK_PRIO,    
                (TaskHandle_t*  )&FR_FunTask_Handler); 
		if(pdPASS == xReturn)
    printf("创建FR_Fun任务成功!\r\n");						
								
		//创建press_FR任务刷指纹						
		xReturn =xTaskCreate((TaskFunction_t )press_FR_task,         
                (const char*    )"press_FR_task",       
                (uint16_t       )press_FR_STK_SIZE, 
                (void*          )NULL,                
                (UBaseType_t    )press_FR_TASK_PRIO,    
                (TaskHandle_t*  )&press_FRTask_Handler); 
    if(pdPASS == xReturn)
    printf("创建press_FR任务成功!\r\n");
		
		//创建LCD180DHT11任务人脸识别通信					
		xReturn =xTaskCreate((TaskFunction_t )LCD180DHT11_task,         
                (const char*    )"LCD180DHT11_task",       
                (uint16_t       )LCD180DHT11_STK_SIZE, 
                (void*          )NULL,                
                (UBaseType_t    )LCD180DHT11_TASK_PRIO,    
                (TaskHandle_t*  )&LCD180DHT11Task_Handler); 
    if(pdPASS == xReturn)
    printf("创建LCD180DHT11任务成功!\r\n");
		
		//创建LCD180MQ2任务人脸识别通信					
		xReturn =xTaskCreate((TaskFunction_t )LCD180MQ2_task,         
                (const char*    )"LCD180MQ2_task",       
                (uint16_t       )LCD180MQ2_STK_SIZE, 
                (void*          )NULL,                
                (UBaseType_t    )LCD180MQ2_TASK_PRIO,    
                (TaskHandle_t*  )&LCD180MQ2Task_Handler); 
    if(pdPASS == xReturn)
    printf("创建LCD180MQ2任务成功!\r\n");
		
		
				//创建LCD任务通信					
		xReturn =xTaskCreate((TaskFunction_t )LCD_task,         
                (const char*    )"LCD_task",       
                (uint16_t       )LCD_STK_SIZE, 
                (void*          )NULL,                
                (UBaseType_t    )LCD_TASK_PRIO,    
                (TaskHandle_t*  )&LCDTask_Handler); 
    if(pdPASS == xReturn)
    printf("创建LCD任务成功!\r\n");
		
		//创建Facedo任务人脸识别通信					
		xReturn =xTaskCreate((TaskFunction_t )FaceDo_task,         
                (const char*    )"FaceDo_task",       
                (uint16_t       )FaceDo_STK_SIZE, 
                (void*          )NULL,                
                (UBaseType_t    )FaceDo_TASK_PRIO,    
                (TaskHandle_t*  )&FaceDoTask_Handler); 
    if(pdPASS == xReturn)
    printf("创建FaceDo任务成功!\r\n");
		
		//创建TEST照相机通信					
//		xReturn =xTaskCreate((TaskFunction_t )TEST_task,         
//                (const char*    )"TEST_task",       
//                (uint16_t       )TEST_STK_SIZE, 
//                (void*          )NULL,                
//                (UBaseType_t    )TEST_TASK_PRIO,    
//                (TaskHandle_t*  )&TESTTask_Handler); 
//    if(pdPASS == xReturn)
//    printf("创建TEST任务成功!\r\n");
		
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

//指纹主任务任务函数 FRMAIN
void FRMAIN_task(void *pvParameters)
{
	u8 ensure;
	u8 key_num;
	BaseType_t err;

	char *str;
	/*提醒用户语句*/
	printf("AS608 Fingerprint module test\r\n");
	printf("Connect with AS608.....\r\n");
	while(PS_HandShake(&AS608Addr))//与AS608模块握手
	{
		printf("Cannot connect with AS608!\r\n");
		delay_xms(10);
		printf("Try to connect again....\r\n");
		delay_xms(10);	  
	}
	//连接模块成功，初始化
	printf("Connect Success!\r\n");
	str=mymalloc(30);
	sprintf(str,"Baudrate:%d   Addr:%x\n",usart2_baund,AS608Addr);//显示波特率
	printf("%s\n",str);
	delay_xms(100);
	ensure=PS_ValidTempleteNum(&ValidN);//读库指纹个数
	if(ensure!=0x00)
		ShowErrMessage(ensure);//显示确认码错误信息	
	ensure=PS_ReadSysPara(&AS608Para);  //读AS608模块参数 
	if(ensure==0x00)
	{
		mymemset(str,0,50);
		sprintf(str,"RemainNum:%d    Level:%d \n",AS608Para.PS_max-ValidN,AS608Para.PS_level);//显示剩余指纹数量和安全等级
		printf("%s\n",str);
	}
	else
		ShowErrMessage(ensure);	
	myfree(str);

    while(1)
    {		
			if((press_FRTask_Handler!=NULL)&&(PS_Sta))
			{
				err=xTaskNotify((TaskHandle_t )press_FRTask_Handler, //任务句柄 (1)
			 (uint32_t )PS_Sta, //任务通知值
			 (eNotifyAction )eSetValueWithOverwrite); //覆写的方式
				if(err==pdFAIL)
				{
				printf("任务通知发送失败\r\n");
				}						
			}
			
			key_num=KEY_Scan(0);//指纹
			if((FR_FunTask_Handler!=NULL)&&(key_num))
			{
				err=xTaskNotify((TaskHandle_t )FR_FunTask_Handler, //任务句柄 (1)
			 (uint32_t )key_num, //任务通知值
			 (eNotifyAction )eSetValueWithOverwrite); //覆写的方式
				if(err==pdFAIL)
				{
				printf("任务通知发送失败\r\n");
				}						
			}
//				key_num=KEY_Scan(0);
//			if((FaceDoTask_Handler!=NULL)&&(key_num))
//			{
//				err=xTaskNotify((TaskHandle_t )FaceDoTask_Handler, //任务句柄 (1)
//			 (uint32_t )key_num, //任务通知值
//			 (eNotifyAction )eSetValueWithOverwrite); //覆写的方式
//				if(err==pdFAIL)
//				{
//				printf("任务通知发送失败\r\n");
//				}						
//			}
			
    }
}

//指纹功能集合
void FR_Fun_task(void *pvParameters){
	u8 num;
	uint32_t NotifyValue;
  BaseType_t err;
	
	while(1){
		err=xTaskNotifyWait((uint32_t )0x00, //进入函数的时候不清除任务 bit (2)
												(uint32_t )ULONG_MAX,//退出函数的时候清除所有的 bit
												(uint32_t* )&NotifyValue, //保存任务通知值
 											  (TickType_t )portMAX_DELAY); //阻塞时间
		if(err==pdTRUE) //获取任务通知成功
		{
			switch((u8)NotifyValue){
				case KEY0_PRES:
					 Add_FR();
				   break;
			  case KEY1_PRES:
					Del_FR();
				  break;
	    }
		}
	}
}

//LCD任务函数
void LCD_task(void *pvParameters){
	u8 res;							 
	u8 *pname;				//带路径的文件名 
	u8 key;					//键值		   
	u8 i;						 
	u8 sd_ok=1;				//0,sd卡不正常;1,SD卡正常.
	
	POINT_COLOR=RED;      
 	while(font_init()) 				//检查字库
	{	    
		LCD_ShowString(30,50,200,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(30,50,240,66,WHITE);//清除显示	     
	}  	 
	Show_Str(30,50,200,16,"精英STM32F1开发板",16,0);				    	 
	Show_Str(30,70,200,16,"照相机实验",16,0);				    	 
	Show_Str(30,90,200,16,"KEY4:拍照",16,0);				    	 
	Show_Str(30,110,200,16,"正点原子@ALIENTEK",16,0);				    	 
	Show_Str(30,130,200,16,"2019年10月9日",16,0);
	res=f_mkdir("0:/PHOTO");		//创建PHOTO文件夹
	if(res!=FR_EXIST&&res!=FR_OK) 	//发生了错误
	{		    
		Show_Str(30,150,240,16,"SD卡错误!",16,0);
		delay_ms(200);				  
		Show_Str(30,170,240,16,"拍照功能将不可用!",16,0);
		sd_ok=0;  	
	}else
	{
		Show_Str(30,150,240,16,"SD卡正常!",16,0);
		delay_ms(200);				  
		Show_Str(30,170,240,16,"KEY4:拍照",16,0);
		sd_ok=1;    	  
	}										   						    
 	pname=mymalloc(30);	//为带路径的文件名分配30个字节的内存		    
 	while(pname==NULL)			//内存分配出错
 	{	    
		Show_Str(30,190,240,16,"内存分配失败!",16,0);
		delay_ms(200);				  
		LCD_Fill(30,190,240,146,WHITE);//清除显示	     
		delay_ms(200);				  
	}   											  
	while(OV7725_Init())//初始化OV7725
	{
		Show_Str(30,190,240,16,"OV7725 错误!",16,0);
		delay_ms(200);
	    LCD_Fill(30,190,239,206,WHITE);
		delay_ms(200);
	}
		Show_Str(30,190,200,16,"OV7725 正常",16,0);
	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY2_PRES)
		{
			OV7725_Window_Set(320,240,0);//QVGA模式输出
			break;
		}else if(key==KEY3_PRES)
		{
			OV7725_Window_Set(320,240,1);//VGA模式输出
			break;
		}
		i++;
		if(i==100)LCD_ShowString(30,206,200,16,16,"KEY2:QVGA  KEY3:VGA"); //闪烁显示提示信息
		if(i==200)
		{	
			LCD_Fill(30,206,200,250+16,WHITE);
			i=0; 
		}
		delay_ms(5);
	}				
	OV7725_CS=0;					 
	TIM6_Int_Init(10000,7199);			//10Khz计数频率,1秒钟中断									  
	EXTI8_Init();						//使能定时器捕获
	LCD_Clear(BLACK);
 	while(1)
	{	
		key=KEY_Scan(0);//不支持连按
		if(key==KEY4_PRES)
		{
			if(sd_ok)
			{
				//LED1=0;	//点亮DS1,提示正在拍照
				camera_new_pathname(pname);//得到文件名		    
				if(bmp_encode(pname,(lcddev.width-240)/2,(lcddev.height-320)/2,240,320,0))//拍照有误
				{
					Show_Str(40,130,240,12,"写入文件错误!",12,0);		 
				}else 
				{
					Show_Str(40,130,240,12,"拍照成功!",12,0);
					Show_Str(40,150,240,12,"保存为:",12,0);
 					Show_Str(40+42,150,240,12,pname,12,0);		    
 					//BEEP=1;	//蜂鸣器短叫，提示拍照完成
					Beep_1();
					delay_ms(100);
		 		}
			}else //提示SD卡错误
			{					    
				Show_Str(40,130,240,12,"SD卡错误!",12,0);
 				Show_Str(40,150,240,12,"拍照功能不可用!",12,0);			    
 			}
 		 	//BEEP=0;//关闭蜂鸣器
			//LED1=1;//关闭DS1
			delay_ms(1800);//等待1.8秒钟
			LCD_Clear(BLACK);
		}else delay_ms(5);
 		camera_refresh();//更新显示
		i++;
		if(i==40)//DS0闪烁.
		{
			i=0;
			//LED0=!LED0;
 		}
	}	   										    
}

//LCD180dht11任务函数
void LCD180DHT11_task(void *pvParameters){
	LCD180_Clear(WHITE);
		while(1){
	u8 t=0;			    
	u8 temperature;  	    
	u8 humidity; 
	Gui_StrCenter(0,23,RED,BLUE,"DHT11TEST",16,1);//居中显示
	while(DHT11_Init())	//DHT11初始化	
	{
		Gui_StrCenter(0,23,RED,BLUE,"DHT11 Error",16,1);
		delay_xms(200);
	}
	Gui_StrCenter(0,57,GREEN,BLUE,"DHT11 OK",16,1);//居中显示
	Gui_StrCenter(0,74,GREEN,BLUE,"Temp:    C",16,1);//居中显示
	Gui_StrCenter(0,91,GREEN,BLUE,"Humi:    %",16,1);//居中显示

		while(1)
	{	    	    
 		if(t%10==0)			//每100ms读取一次
		{									  
			DHT11_Read_Data(&temperature,&humidity);	//读取温湿度值				
			//USART_SendData(USART1,temperature);
			//USART_SendData(USART1,humidity);
			LCD180_ShowNum(70,74,temperature,2,16);	//显示温度	  	
			LCD180_ShowNum(70,91,humidity,2,16);		//显示湿度	

		}				   
	 	delay_xms(10);
		t++;
		if(t==20)
		{
			t=0;
		}
	}
	}
}	

//LCD180MQ2任务函数
void LCD180MQ2_task(void *pvParameters){
	
		float vol;//存放原始的电压值
    float mq2ppm;//存放气体浓度
		u16 adcx;//adc采集数据，4096份
	  LCD180_Clear(WHITE);
	  Gui_StrCenter(0,108,BLUE,BLUE,"PPM:    ",16,1);//居中显示
	  Gui_StrCenter(0,125,BLUE,BLUE,"vol:    ",16,1);//居中显示
	  while(1){
		vol=(adcx*3.3)/4096;//电压值
		
			mq2ppm = MQ2_GetPPM();
//			printf("MQ2PPM:%0.1f",mq2ppm);
//			printf("VOL:%0.1f",vol);
//			delay_ms(500);
		LCD180_ShowNum(70,108,mq2ppm,2,16);		//显示烟雾浓度
		LCD180_ShowNum(70,125,vol,2,16);		//显示烟雾浓度
	}
}

   //TEST任务函数
//void TEST_task(void *pvParameters){
//	u8 temperature;  	    
//	u8 humidity; 
//	float mq2ppm;//存放气体浓度
//	u8 t=0;
//	while(1){
//								  
//			DHT11_Read_Data(&temperature,&humidity);	//读取温湿度值				
//			mq2ppm = MQ2_GetPPM();
//	if(temperature>=25){
//		GPIO_SetBits(GPIOE,GPIO_Pin_0);
//	}
//	
//	}
//	
//}

//人脸识别通信do
void FaceDo_task(void *pvParameters){
	u8 num;
	uint32_t NotifyValue;
  BaseType_t err;
	
	while(1){
//		err=xTaskNotifyWait((uint32_t )0x00, //进入函数的时候不清除任务 bit (2)
//												(uint32_t )ULONG_MAX,//退出函数的时候清除所有的 bit
//												(uint32_t* )&NotifyValue, //保存任务通知值
// 											  (TickType_t )portMAX_DELAY); //阻塞时间
//		if(err==pdTRUE) //获取任务通知成功
//		{
//			switch((u8)NotifyValue){
//				case Face_PRES:
//					 Face();
//				   break;
//	    }
//		}
		Face();
		vTaskDelay(5);
	}
}


//刷指纹功能
void press_FR_task(void *pvParameters){
	uint32_t NotifyValue;
	BaseType_t err;
	SearchResult seach;
	u8 ensure;
	char *str;
	
	while(1){
		err=xTaskNotifyWait((uint32_t )0x00, //进入函数的时候不清除任务 bit (2)
												(uint32_t )ULONG_MAX,//退出函数的时候清除所有的 bit
												(uint32_t* )&NotifyValue, //保存任务通知值
 											  (TickType_t )portMAX_DELAY); //阻塞时间
		if(err==pdTRUE) //获取任务通知成功
		{
		  if(PS_Sta)	 //检测PS_Sta状态，如果有手指按下
		  {
			press_FR();//刷指纹
  	  }
		}
	}
}


//录指纹
void Add_FR(void)
{
	u8 i=0,ensure ,processnum=0;
	u8 len1;
	u8 t1;
	u8 a1;
	while(1)
	{
		switch (processnum)
		{
			case 0:
				i++;
				printf("请按手指！\r\n");
			  
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer1);//生成特征
					if(ensure==0x00)
					{
						//指纹正确
						printf("指纹正确\r\n");
						i=0;
						processnum=1;//跳到第二步						
					}else ShowErrMessage(ensure);				
				}else ShowErrMessage(ensure);						
				break;
			
			case 1:
				i++;
				printf("再按一次手指\r\n");
			
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure= PS_GenChar(CharBuffer2);//生成特征			
					if(ensure==0x00)
					{
						printf("指纹正确\r\n");
						i=0; 
						processnum=2;//跳到第三步
					}else ShowErrMessage(ensure);	
				}else ShowErrMessage(ensure);		
				break;

			case 2:
				printf("对比两次指纹\r\n");
				ensure=PS_Match();
				if(ensure==0x00) 
				{
					printf("两次指纹对比一样\r\n");
					processnum=3;//跳到第四步
				}
				else 
				{
					printf("对比失败，请重按手指！\r\n");
					ShowErrMessage(ensure);
					i=0;
					processnum=0;//跳回第一步		
				}
				delay_ms(1000);
				break;

			case 3:
				printf("产生一个指纹模块\r\n");
				ensure=PS_RegModel();
				if(ensure==0x00) 
				{
					printf("生成指纹模板成功\r\n");
					processnum=4;//跳到第五步
				}else {processnum=0;ShowErrMessage(ensure);}
				delay_ms(1000);
				break;
				
			case 4:	
				printf("请输入该指纹模块存放的位置（必须小于300）：\r\n");
				while(1)
				{
					if(USART_RX_STA&0X8000)
					{
						len1=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
						printf("\r\n您发送的位置为:\r\n");
						for(a1=0;a1<len1;a1++)
						{
								a1=a1*10+USART_RX_BUF[a1]-'0';
						}		
							a1=a1-1;
					
					for(t1=0;t1<len1;t1++)
					{
						USART1->DR=USART_RX_BUF[t1];
						while((USART1->SR&0X40)==0);//等待发送结束
					}
						printf("\r\n\r\n");//插入换行
						USART_RX_STA=0;
					}
					if(a1>0)
						break;
				}				
				ensure=PS_StoreChar(CharBuffer2,a1);//储存模板
				if(ensure==0x00) 
				{			

					printf("添加指纹成功！\r\n");
					PS_ValidTempleteNum(&ValidN);//读库指纹个数
					//显示剩余指纹个数
					printf("剩余指纹容量:%d\r\n",AS608Para.PS_max-ValidN);
					delay_ms(1500);//延时后清除显示	
					printf("请继续你的操作！\r\n");
					return ;
				}else {processnum=0;ShowErrMessage(ensure);}					
				break;				
		}
		delay_ms(500);
		if(i==5)//超过5次没有按手指则退出
		{
			printf("现在退出了！\r\n");
			break;	
		}				
	}
}

//删除指纹功能
void Del_FR(void)
{
	u8  ensure;
	u8 len;	
	u8 t;
	u8 a;
	printf("删除指纹：\r\n");
	printf("请输入要删除指纹的位置:(范围：0=< ID <=299)\r\n");
	
	while(1)
	{
		//接受串口信息的核心部分代码！
		if(USART_RX_STA&0X8000)
		{
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			printf("\r\n您发送的位置为:\r\n");
			
			//下面代码的功能是将串口缓冲区的数组，转换成int，默认+1.所以-1
			for(a=0;a<len;a++)
			{
					a=a*10+USART_RX_BUF[a]-'0';
			}
			a=a-1;
			for(t=0;t<len;t++)
				{
					USART1->DR=USART_RX_BUF[t];
					while((USART1->SR&0X40)==0);//等待发送结束
				}
				printf("\r\n\r\n");//插入换行
				USART_RX_STA=0;
		}
		if(a>0)
			break;
	}
	//如果接收到的是0 则清空指纹库，其他位置删除对应位置的指纹，该功能目前不能用！前面判断依据退出了
	if(a==0)
		ensure=PS_Empty();//清空指纹库
	else 
		ensure=PS_DeletChar(a,1);//删除单个指纹
	if(ensure==0)
	{
		printf("删除指纹成功\r\n");
	}
  else
		ShowErrMessage(ensure);	
	delay_ms(1500);//延时后清除显示
	PS_ValidTempleteNum(&ValidN);//读库指纹个数
	printf("剩下指纹数量：%d\r\n",AS608Para.PS_max-ValidN);
	delay_ms(50);
	printf("请继续你的操作！\r\n");
	return;
}

//刷指纹功能
void press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	char *str;
	ensure=PS_GetImage();
	if(ensure==0x00)//获取图像成功 
	{	
		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //生成特征成功
		{		
			ensure=PS_HighSpeedSearch(CharBuffer1,0,300,&seach);
			if(ensure==0x00)//搜索成功
			{					
				printf("搜索指纹成功\r\n");				
				str=mymalloc(50);
				sprintf(str,"Match ID:%d  Match score:%d\n",seach.pageID,seach.mathscore);//显示匹配指纹的ID和分数
				printf("%s\r\n",str);
				doSG90();//SG90即开门
				myfree(str);

			}
			else  //刷指纹错误 蜂鸣器响一次
				Beep_1();
				ShowErrMessage(ensure);					
	  }
		else
			ShowErrMessage(ensure);
	 delay_ms(1000);
	}	
}

//SG90舵机工作
void doSG90(void){
	while(1)
   {
		 TIM_SetCompare2(TIM3,175);
		 delay_ms(1000);
		 printf("正在运行\r\n");
		 delay_ms(1000);
		 TIM_SetCompare2(TIM3,195);//500/20000=0.5/20
		 printf("运行完成\r\n");
		 break;
	 } 
//t = 0.5ms——————-舵机会转动 0 °
//t = 1.0ms——————-舵机会转动 45°
//t = 1.5ms——————-舵机会转动 90°
//t = 2.0ms——————-舵机会转动 135°
//t = 2.5ms——————-舵机会转动180°
}
//警报1次
void Beep_1(void){
				BEEP=1;
				delay_ms(300);
				BEEP=0;
}
//警报2次
void Beep_2(void){
	BEEP=1;
				delay_ms(300);
				BEEP=0;
	BEEP=1;
				delay_ms(300);
				BEEP=0;
} 
//警报3次
void Beep_3(void){
BEEP=1;
				delay_ms(300);
				BEEP=0;
	BEEP=1;
				delay_ms(300);
				BEEP=0;
	BEEP=1;
				delay_ms(300);
				BEEP=0;
}

 //人脸识别
void Face(void){
	if(USART3_RX_STA){
		printf("USART3_RX_BUF:%s\r\n",USART3_RX_BUF);
	if(USART3_RX_BUF[14]==0x59) //ascii码表的值 16进制 0x31==十进制的1 
		{
			doSG90();//SG90即开门
			memset(USART3_RX_BUF, 0, sizeof(USART3_RX_BUF));
			count=0;
			USART3_RX_STA=0;
		}
		else{
			if(USART3_RX_BUF[14]==0x4E){
				Beep_1();
				printf("face fail\r\n");
			}
			memset(USART3_RX_BUF, 0, sizeof(USART3_RX_BUF));
			count=0;
			USART3_RX_STA=0;
		}
	}
}

void camera_refresh(void)
{
	u32 j;
 	u16 color;	 
	if(ov_sta)//有帧中断更新？
	{
		LCD_Scan_Dir(U2D_L2R);		//从上到下,从左到右  
		if(lcddev.id==0X1963)LCD_Set_Window((lcddev.width-240)/2,(lcddev.height-320)/2,240,320);//将显示区域设置到屏幕中央
		else if(lcddev.id==0X5510||lcddev.id==0X5310)LCD_Set_Window((lcddev.width-320)/2,(lcddev.height-240)/2,320,240);//将显示区域设置到屏幕中央
		LCD_WriteRAM_Prepare();     //开始写入GRAM	
		OV7725_RRST=0;				//开始复位读指针 
		OV7725_RCK_L;
		OV7725_RCK_H;
		OV7725_RCK_L;
		OV7725_RRST=1;				//复位读指针结束 
		OV7725_RCK_H; 
		for(j=0;j<76800;j++)
		{
			OV7725_RCK_L;
			color=GPIOC->IDR&0XFF;	//读数据
			OV7725_RCK_H; 
			color<<=8;  
			OV7725_RCK_L;
			color|=GPIOC->IDR&0XFF;	//读数据
			OV7725_RCK_H; 
			LCD->LCD_RAM=color;    
		}   							  
 		ov_sta=0;					//清零帧中断标记
		ov_frame++; 
		LCD_Scan_Dir(DFT_SCAN_DIR);	//恢复默认扫描方向 
	} 
}   
 
//文件名自增（避免覆盖）
//组合成:形如"0:PHOTO/PIC13141.bmp"的文件名
void camera_new_pathname(u8 *pname)
{	 
	u8 res;					 
	u16 index=0;
	while(index<0XFFFF)
	{
		sprintf((char*)pname,"0:PHOTO/PIC%05d.bmp",index);
		res=f_open(ftemp,(const TCHAR*)pname,FA_READ);//尝试打开这个文件
		if(res==FR_NO_FILE)break;		//该文件名不存在=正是我们需要的.
		index++;
	}
}