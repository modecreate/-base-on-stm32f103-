#ifndef _ESP32_H_
#define _ESP32_H_





#define REV_OK		0	//接收完成标志
#define REV_WAIT	1	//接收未完成标志


void ESP32_Init(void);

void ESP32_Clear(void);

void ESP32_SendData(unsigned char *data, unsigned short len);

unsigned char *ESP32_GetIPD(unsigned short timeOut);


#endif
