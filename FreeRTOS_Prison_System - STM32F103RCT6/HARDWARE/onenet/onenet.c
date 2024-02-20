#include "esp32.h"
#include "onenet.h"
#include "usart.h"
#include "delay.h"
#include "mqttKit.h"
#include "string.h"
#include "led.h"
#include "dht11.h"
//#include "mq9.h"

//延时API
#define ONENET_DELAYMS   delay_ms
#define ONENET_DELAYUS   delay_us

//接入参数--Mqtt 产品
#define ONENET_MQTT_PRO  "460402"  //产品ID
#define ONENET_MQTT_DEV  "793987873"  //设备ID
#define ONENET_MQTT_AUT  "321"  //鉴权信息
#define ONENET_MQTT_API  "PvSnB6DeZFIItN10Bbetqv9XfJQ=="  //API
//Mqtt上传DS18B20检测温度
#define ONENET_MQTT_UP_DSTEMP  0x1820
//Mqtt上传DHT11检测温湿度
#define ONENET_MQTT_UP_DHT11   0xDD11
//使能Mqtt上传温湿度类传感器——默认功能DS和DHT二选一
#define ONENET_MQTT_TSEN ONENET_MQTT_UP_DHT11


/*********************************************************************
 * FunctionName : MqttLinkOneNet()
 * Description  : 与OneNet进行MQTT连接
 * Parameters   : void
 * Returns      : uint8_t 1-成功 0-失败
*********************************************************************/
uint8_t MqttLinkOneNet(void) {
  MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};//协议包
  uint8_t *dataPtr;
  uint8_t status = 1;
	
  printf("OneNet_DevLink\r\n"
    "PROID: %s,	AUIF: %s,	DEVID:%s\r\n", 
    ONENET_MQTT_PRO, ONENET_MQTT_AUT, ONENET_MQTT_DEV);
	
  if(MQTT_PacketConnect(ONENET_MQTT_PRO, 
        ONENET_MQTT_AUT, ONENET_MQTT_DEV, 
        256, 0, MQTT_QOS_LEVEL0, NULL, NULL, 0, &mqttPacket) == 0) {
    ESP32_SendData(mqttPacket._data, mqttPacket._len);//上传平台
    dataPtr = ESP32_GetIPD(250);//等待平台响应
    if(dataPtr != NULL) {
      if(MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_CONNACK) {  
        switch(MQTT_UnPacketConnectAck(dataPtr)) {
          case 0:printf("Tips:	连接成功\r\n");status = 0;break;
          case 1:printf("WARN:	连接失败：协议错误\r\n");break;
          case 2:printf("WARN:	连接失败：非法的clientid\r\n");break;
          case 3:printf("WARN:	连接失败：服务器失败\r\n");break;
          case 4:printf("WARN:	连接失败：用户名或密码错误\r\n");break;
          case 5:printf("WARN:	连接失败：非法链接(比如token非法)\r\n");break;					
          default:printf("ERR:	连接失败：未知错误\r\n");break;
        }
      }
    }
    MQTT_DeleteBuffer(&mqttPacket);//删包
  } else {
    printf("WARN:	MQTT_PacketConnect Failed\r\n");
  }	
	return status;
}
/*********************************************************************
 * FunctionName : PackMsgInType3OneNet()
 * Description  : 按OneNet格式3进行数据封装
 * Parameters   : char *buf  ——> 用于封装的缓冲基址
 * Returns      : uint8_t 封装后长度
*********************************************************************/

 
uint8_t PackMsgInType3OneNet(char *buf) {
  char text[16];
	
  	uint8_t temp=0;//温度值
		uint8_t humi=0;//湿度值
	u8 LED1_Status;
	u8 LED0_Status;
	u8 FAN0_Status;
	u8 FAN1_Status;

	 DHT11_Read_Data(&temp, &humi);
		printf("temp:%d\r\n",temp);
		printf("humi:%d\r\n",humi);	
	LED1_Status=get_LED1_status();
	LED0_Status=get_LED0_status();
	FAN0_Status=get_FAN0_status();
	FAN1_Status=get_FAN1_status();
	printf("LED1_Status:%d\r\n",LED1_Status);
	printf("LED0_Status:%d\r\n",LED0_Status);	
	printf("FAN0_Status:%d\r\n",FAN0_Status);	
	printf("FAN1_Status:%d\r\n",FAN1_Status);	
	
	memset ( text, 0, sizeof ( text ) );
	
	strcat ( buf, "{" );
	
	memset ( text, 0, sizeof ( text ) );
	sprintf ( text, "\"LED1\":%d,", LED1_Status );
	strcat ( buf, text );
	
	memset ( text, 0, sizeof ( text ) );
	sprintf ( text, "\"LED0\":%d,", LED0_Status );
	strcat ( buf, text );
	
	memset ( text, 0, sizeof ( text ) );
	sprintf ( text, "\"FAN0\":%d,", FAN0_Status );
	strcat ( buf, text );
	
	memset ( text, 0, sizeof ( text ) );
	sprintf ( text, "\"FAN1\":%d,", FAN1_Status );
	strcat ( buf, text );
	
	memset ( text, 0, sizeof ( text ) );
	sprintf ( text, "\"temp\":%d,", temp );
	strcat ( buf, text );

	
	memset ( text, 0, sizeof ( text ) );
	sprintf ( text, "\"humi\":%d", humi );
	strcat ( buf, text );


	strcat ( buf, "}" );

	return strlen ( buf );
}
/*********************************************************************
 * FunctionName : MqttSendDataOneNet()
 * Description  : 封装MQTT数据包并上传OneNet
 * Parameters   : void
 * Returns      : void
*********************************************************************/
void MqttSendDataOneNet(void) {	
  MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};//协议包
  char buf[128];
  short body_len = 0, i = 0;
  printf("Tips:	OneNet_SendData-MQTT\r\n");
  memset(buf, 0, sizeof(buf));
  body_len = PackMsgInType3OneNet(buf);//获取发送数据流总长度
	
  if(body_len) {
    if(MQTT_PacketSaveData(ONENET_MQTT_DEV, 
          body_len, NULL, 3, &mqttPacket) == 0) {
      for(; i < body_len; i++)
        mqttPacket._data[mqttPacket._len++] = buf[i];
			
      ONENET_MQTT_UP(mqttPacket._data, mqttPacket._len);									//上传数据到平台
      printf("Send %d Bytes\r\n", mqttPacket._len);
			
      MQTT_DeleteBuffer(&mqttPacket);															//删包
    } else {
      printf("WARN:	MQTT_NewBuffer Failed\r\n");
    }
	}	
}
/*********************************************************************
 * FunctionName : MqttRecvProOneNet()
 * Description  : 收到OneNet的MQTT数据包处理函数
 * Parameters   : uint8_t *cmd  ——> 收到数据基址
 * Returns      : void
*********************************************************************/
void MqttRecvProOneNet(uint8_t *cmd) {
  MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};//协议包	
  char *req_payload = NULL;
  char *cmdid_topic = NULL;	
  unsigned short req_len = 0;	
  unsigned char type = 0;	
  short result = 0;
  char *dataPtr = NULL;
  char numBuf[10];
  int num = 0;
	
  type = MQTT_UnPacketRecv(cmd);
	switch(type) {
    case MQTT_PKT_CMD:   //命令下发		
      result = MQTT_UnPacketCmd(cmd, &cmdid_topic, 
                 &req_payload, &req_len);	//解出topic和消息体
      if(result == 0) {
        printf("cmdid: %s, req: %s, req_len: %d\r\n", 
                 cmdid_topic, req_payload, req_len);
        if(MQTT_PacketCmdResp(cmdid_topic, 
               req_payload, &mqttPacket) == 0) {//命令回复组包
          printf("Tips:	Send CmdResp\r\n");
          ONENET_MQTT_UP(mqttPacket._data, mqttPacket._len);
          MQTT_DeleteBuffer(&mqttPacket);
        }
      }		
    break;
			
    case MQTT_PKT_PUBACK://发送Publish消息，平台回复的Ack		
      if(MQTT_UnPacketPublishAck(cmd) == 0)
        printf("Tips:	MQTT Publish Send OK\r\n");			
    break;
		
    default:
      result = -1;
    break;
  }
	
  ESP32_Clear(); //清空缓存	
  if(result == -1) return;
	
  dataPtr = strchr(req_payload, ':');//搜索':'

  if(dataPtr != NULL && result != -1) {
    dataPtr++;
  //判断是否是下发的命令控制数据		
    while(*dataPtr >= '0' && *dataPtr <= '9')
      numBuf[num++] = *dataPtr++;
    num = atoi((const char *)numBuf);//转为数值形式
  //搜索"led1"		
		 if(strstr((char *)req_payload, "led1")) {
      if(num == 0)     //控制数据如果为1，代表开			
        LED1=1;
      else if(num == 1)//控制数据如果为0，代表关
        LED1=0;
    } else if(strstr((char *)req_payload, "led0")) {
      if(num == 0)
        LED0=1;
      else if(num == 1)
        LED0=0;
    } else if(strstr((char *)req_payload, "fan0")) {
      if(num == 0)
        FAN0=0;
      else if(num == 1)
        FAN0=1;
    } else if(strstr((char *)req_payload, "fan1")) {
      if(num == 0)
        FAN1=0;
      else if(num == 1)
        FAN1=1;
    }
    
  }

  if(type == MQTT_PKT_CMD || type == MQTT_PKT_PUBLISH) {
    MQTT_FreeBuffer(cmdid_topic);
    MQTT_FreeBuffer(req_payload);
  }

}

