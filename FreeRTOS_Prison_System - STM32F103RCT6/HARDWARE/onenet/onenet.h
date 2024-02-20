#ifndef _ONENET_H_
#define _ONENET_H_
#include "sys.h"

#define ONENET_MQTT_UP       ESP32_SendData

uint8_t MqttLinkOneNet(void);
void MqttSendDataOneNet(void);
void MqttRecvProOneNet(uint8_t *cmd);
#endif
