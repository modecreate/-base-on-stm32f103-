实验器材:
	Mini STM32F103开发板
	
实验目的:
	学习ATK-AS608指纹识别模块的使用,学会录指纹、删除指纹、设置波特率及对比等级等功能.
	
硬件资源:	
	1,GPIO（PA6）读取指纹模块触摸感应信号
	2,按键KEY0(PE4)/KEY1(PE3)/KEY2(PE2)/KEY_UP(PA0,也称之为WK_UP)
	3,串口1(波特率:115200,PA9/PA10连接在板载USB转串口芯片CH340上面)用于支持USMART调试
	4,串口2(波特率:57600,PA2/PA3)与指纹模块AS608通讯
	5,ALIENTEK 2.8/3.5/4.3/7寸TFTLCD模块(通过FSMC驱动,FSMC_NE4接LCD片选/A6接RS) 	
	6,W25Q128(SPI FLASH芯片,连接在SPI1上，用于存储字库) 
	7,24C02（用于存储LCD触摸屏一些参数）
	8,ATK-AS608指纹识别模块1个,杜邦线6根
	
	ATK-AS608模块与开发板连接方式1（通过6根杜邦线连接）：
	ATK-AS608          STM32开发板
		3.3V<----------->3.3V 
		TX<------------->PA3(RX)
		RX <------------>PA2(TX)
		GND<------------>GND 
		touch<---------->PA6
		touchVin<------>3.3V			

实验现象:	
	本实验功能简介：本实验用于测试ATK-AS608模块，总共包括三项测试：
	
	1，删指纹——与模块握手通过之后，屏幕将虚拟一个键盘，按‘del_fp’进入删除界面。在此界面可
	   以按照提示输出一个数值ID按‘Enter’删除单个指纹，也可按‘del_all’清空指纹库。如果是误
	   操作也可以按‘BACK’返回主界面。

	2，录指纹——按‘add_fp’进入此项。录指纹流程：按第一次手指-->按第二次手指-->自动生成模板
	   -->输入存储ID-->存储指纹成功。其中两次按指纹操作，如果超过5次无手指按下则退回主界面。
	   模块感应到手指按下0.3S（最少停留0.3S）后LCD提示指纹是否正常！

	3，刷指纹--与模块握手成功之后，主函数中判断指纹模块的感应引脚（touch->PA6）模块感应到手指
	   输出高电平，程序判断后进入刷指纹流程。读指纹图像-->搜索匹配指纹库-->成功或没有搜到指纹。


错误记录：
1.跳static void prvTaskExitError( void )
{
    configASSERT( uxCriticalNesting == ~0UL );
    portDISABLE_INTERRUPTS();

    for( ; ; )
    {
    }
}
解决：
1）从上面可以看出prvTaskExitError被赋值给了LR寄存器，也就是STM32的R14寄存器，这个寄存器是保存函数的返回地址的，就是函数被调用完成之后返回原来的位置。
pxPortInitialiseStack是任务建立xTaskGenericCreate的时候，堆栈初始化的。
分析看，任务退出的时候调用了prvTaskExitError函数，但是正常任务是个死循环，就算删除，也是调用删除函数，因此有两种情况可能出现
1. 任务没有 while(1)的死循环。
2. while(1)里面有break函数。
经过排查是2的问题，最终解决问题。
出现：
debug时刷指纹后跳到port.c然后不执行

2）知识点：
2.2.1 临界段基本概念
什么是临界段？一句话，不允许任何中断打断或任务切换的代码段。
为何要有临界段？
1，任务间通信全局变量的读取或修改变量。例如：内存操作、校验码等。
2，调用不可重入函数。
缺点：
影响操作系统的实时性。如果在临界代码段有中断发生，中断不会立即执行，对于某些系统可能会引起严重后果。
临界段：
少用。如果一定要用这段代码一定要执行时间短，更不能有引发死循环的逻辑操作
解决：
刷脸的时候一定要开发板的摄像头对准人脸，否则识别不出来，会一直显示11111

3）串口通信判断时，对比上位机传给下位机的数据是否包含或者吻合某个值时，对比完就清空缓存字符串，此处采用memset时数组和字符串不同
memset(USART_RX_BUF, '\0', sizeof(USART_RX_BUF));

4）if(USART3_RX_BUF[0]==0x59) //ascii码表的值 16进制 0x31==十进制的1 
		{
			printf("USART3_RX_BUF:%s\r\n",&USART3_RX_BUF);
			memset(USART3_RX_BUF, '0', sizeof(USART3_RX_BUF));
			printf("USART3_RX_BUF:%s\r\n",&USART3_RX_BUF);
			doSG90();//SG90即开门 
		}
只有开机第一次才会接收到k210发来的y，后面都是把y转成0了。如何一直无法接收到k210的y（换种说法就是USART3_RX_BUF里的数据一直是0）
解决：

..\OBJ\AS608.axf: Error: L6406E: No space in execution regions with .ANY selector matching startup_stm32f10x_hd.o(STACK).
内存不够，解决：无法解决

main.c(93): warning:  #223-D: function "LCD_Init" declared implicitly
解决：这种情况下很可能是头文件的预定义声明重复了。比如写了两个LED文件，一个是LED1.c，另一个是LED2.c。他们俩都是从LED.c移植过来的，然后头文件忘了修改，里面的预定义都是
#ifndef __LED_H_
#define __LED_H_

	........

#endif

