ʵ������:
	Mini STM32F103������
	
ʵ��Ŀ��:
	ѧϰATK-AS608ָ��ʶ��ģ���ʹ��,ѧ��¼ָ�ơ�ɾ��ָ�ơ����ò����ʼ��Աȵȼ��ȹ���.
	
Ӳ����Դ:	
	1,GPIO��PA6����ȡָ��ģ�鴥����Ӧ�ź�
	2,����KEY0(PE4)/KEY1(PE3)/KEY2(PE2)/KEY_UP(PA0,Ҳ��֮ΪWK_UP)
	3,����1(������:115200,PA9/PA10�����ڰ���USBת����оƬCH340����)����֧��USMART����
	4,����2(������:57600,PA2/PA3)��ָ��ģ��AS608ͨѶ
	5,ALIENTEK 2.8/3.5/4.3/7��TFTLCDģ��(ͨ��FSMC����,FSMC_NE4��LCDƬѡ/A6��RS) 	
	6,W25Q128(SPI FLASHоƬ,������SPI1�ϣ����ڴ洢�ֿ�) 
	7,24C02�����ڴ洢LCD������һЩ������
	8,ATK-AS608ָ��ʶ��ģ��1��,�Ű���6��
	
	ATK-AS608ģ���뿪�������ӷ�ʽ1��ͨ��6���Ű������ӣ���
	ATK-AS608          STM32������
		3.3V<----------->3.3V 
		TX<------------->PA3(RX)
		RX <------------>PA2(TX)
		GND<------------>GND 
		touch<---------->PA6
		touchVin<------>3.3V			

ʵ������:	
	��ʵ�鹦�ܼ�飺��ʵ�����ڲ���ATK-AS608ģ�飬�ܹ�����������ԣ�
	
	1��ɾָ�ơ�����ģ������ͨ��֮����Ļ������һ�����̣�����del_fp������ɾ�����档�ڴ˽����
	   �԰�����ʾ���һ����ֵID����Enter��ɾ������ָ�ƣ�Ҳ�ɰ���del_all�����ָ�ƿ⡣�������
	   ����Ҳ���԰���BACK�����������档

	2��¼ָ�ơ�������add_fp��������¼ָ�����̣�����һ����ָ-->���ڶ�����ָ-->�Զ�����ģ��
	   -->����洢ID-->�洢ָ�Ƴɹ����������ΰ�ָ�Ʋ������������5������ָ�������˻������档
	   ģ���Ӧ����ָ����0.3S������ͣ��0.3S����LCD��ʾָ���Ƿ�������

	3��ˢָ��--��ģ�����ֳɹ�֮�����������ж�ָ��ģ��ĸ�Ӧ���ţ�touch->PA6��ģ���Ӧ����ָ
	   ����ߵ�ƽ�������жϺ����ˢָ�����̡���ָ��ͼ��-->����ƥ��ָ�ƿ�-->�ɹ���û���ѵ�ָ�ơ�


�����¼��
1.��static void prvTaskExitError( void )
{
    configASSERT( uxCriticalNesting == ~0UL );
    portDISABLE_INTERRUPTS();

    for( ; ; )
    {
    }
}
�����
1����������Կ���prvTaskExitError����ֵ����LR�Ĵ�����Ҳ����STM32��R14�Ĵ���������Ĵ����Ǳ��溯���ķ��ص�ַ�ģ����Ǻ������������֮�󷵻�ԭ����λ�á�
pxPortInitialiseStack��������xTaskGenericCreate��ʱ�򣬶�ջ��ʼ���ġ�
�������������˳���ʱ�������prvTaskExitError�������������������Ǹ���ѭ��������ɾ����Ҳ�ǵ���ɾ�����������������������ܳ���
1. ����û�� while(1)����ѭ����
2. while(1)������break������
�����Ų���2�����⣬���ս�����⡣
���֣�
debugʱˢָ�ƺ�����port.cȻ��ִ��

2��֪ʶ�㣺
2.2.1 �ٽ�λ�������
ʲô���ٽ�Σ�һ�仰���������κ��жϴ�ϻ������л��Ĵ���Ρ�
Ϊ��Ҫ���ٽ�Σ�
1�������ͨ��ȫ�ֱ����Ķ�ȡ���޸ı��������磺�ڴ������У����ȡ�
2�����ò������뺯����
ȱ�㣺
Ӱ�����ϵͳ��ʵʱ�ԡ�������ٽ��������жϷ������жϲ�������ִ�У�����ĳЩϵͳ���ܻ��������غ����
�ٽ�Σ�
���á����һ��Ҫ����δ���һ��Ҫִ��ʱ��̣���������������ѭ�����߼�����
�����
ˢ����ʱ��һ��Ҫ�����������ͷ��׼����������ʶ�𲻳�������һֱ��ʾ11111

3������ͨ���ж�ʱ���Ա���λ��������λ���������Ƿ���������Ǻ�ĳ��ֵʱ���Ա������ջ����ַ������˴�����memsetʱ������ַ�����ͬ
memset(USART_RX_BUF, '\0', sizeof(USART_RX_BUF));

4��if(USART3_RX_BUF[0]==0x59) //ascii�����ֵ 16���� 0x31==ʮ���Ƶ�1 
		{
			printf("USART3_RX_BUF:%s\r\n",&USART3_RX_BUF);
			memset(USART3_RX_BUF, '0', sizeof(USART3_RX_BUF));
			printf("USART3_RX_BUF:%s\r\n",&USART3_RX_BUF);
			doSG90();//SG90������ 
		}
ֻ�п�����һ�βŻ���յ�k210������y�����涼�ǰ�yת��0�ˡ����һֱ�޷����յ�k210��y������˵������USART3_RX_BUF�������һֱ��0��
�����

..\OBJ\AS608.axf: Error: L6406E: No space in execution regions with .ANY selector matching startup_stm32f10x_hd.o(STACK).
�ڴ治����������޷����

main.c(93): warning:  #223-D: function "LCD_Init" declared implicitly
�������������ºܿ�����ͷ�ļ���Ԥ���������ظ��ˡ�����д������LED�ļ���һ����LED1.c����һ����LED2.c�����������Ǵ�LED.c��ֲ�����ģ�Ȼ��ͷ�ļ������޸ģ������Ԥ���嶼��
#ifndef __LED_H_
#define __LED_H_

	........

#endif
