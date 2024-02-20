#ifndef __UART3_H
#define __UART3_H

#include "sys.h"

typedef struct ESP_DATA{
  uint8_t esp_buf[256];
	uint16_t count;
	uint8_t flag;
}esp_dev;
extern esp_dev wifi_dev;

void uart3_init(void);
void uart3_clear(void);
void USART3_SendString(uint8_t *arr,uint16_t len);
#endif
