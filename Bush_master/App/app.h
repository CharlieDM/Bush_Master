#ifndef _APP_H_
#define _APP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "type_def.h"
	
typedef struct APP_data
{   
    struct CMD
	{
		uint8  OpenSwitch;//系统总开关
		uint8  Condition_OpenSwitch;//空调开关
		uint8  ConditionMode;//空调模式
		uint8  WindSpeed;//风速
		uint8  XinFeng_Heat_Flag;//新风电加热标志位
		uint8  XinFeng_Flag;//新排风机标志位
		uint8  JingHua_Flag;//净化标志位
		uint8  Humidity_Flag;//加湿标志位=1加湿；=2除湿；=0无状态；
		uint8  Drain_Pump_Flag;//排水泵标志位
		uint8  State_Change;
	}Set_CMD;      //操作指令	
	struct 	Parameter0
	{
		uint8 XinFeng_sta;
		uint8 Humidity_sta;
		uint8 Drain_PUMP_sta;
		uint8 JingHua_sta;
		uint8 XinFeng_Heat_sta;
	}Task_sta;//系统控制命令执行后，各动作单元状态
	struct Parameter1
	{
		uint8  Temperature;
		uint8  Humidity;
		uint8  CO2;
		uint8  PM25;
		uint8  VOC;
	}Set_Parameter;//外部参数设置窗口
	struct Parameter2
	{
		uint8 Mode;//返回空调当前模式
		uint8 Temperature_T1;//返回空调当前T1温度
		uint8 Temperature_T2;//返回空调当前T2温度
		uint8 WindSpeed;//返回空调当前风速
		uint8 Fault_Mode;//返回空调错误模式
		uint8 Condition_M10_Sta;
	}Condition_Back;//空调返回的数据
		
	uint8 System_Fault;//整个系统报错模式记录
	
}APPdata;

void APP_Init(void);
void APP_Run(void);
#ifdef __cplusplus
}
#endif

#endif /* _app_H_ */
