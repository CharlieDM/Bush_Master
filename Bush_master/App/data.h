#ifndef _data_H_
#define _data_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "type_def.h"

typedef struct APP_Data
{
	struct GUI_data
	{ 
		uint8 Temperature[16];//温度
		uint8 Humidity[16];//温度
		uint16 PM25[16];
		uint16 VOC[16];
		uint16 CO2[16];
		
	}GUI_Data;
	
	struct Host_data
	{
		uint8  a;
	}Host_Data; 
	
	struct Slave_data
	{
		uint16 OnoffMode;
		uint16 HotCoolMode;
		uint16 FanSpeed; 
		uint16 Tempture;
		uint16 XinFeng;
		uint16 AirExhaust;
		uint16 ElecHeating;
		uint16 Humidity;
		uint16 Id;		
	}Slave_Data;
	
  uint8  Mode;//数据模式	
	
}Dataall;
#define Host_Mode     1
#define Slave_Mode    2
#define GUI_Mode      3 
extern Dataall Data_All ;
	
	#ifdef __cplusplus
}
#endif
#endif