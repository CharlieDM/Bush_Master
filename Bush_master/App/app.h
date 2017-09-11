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
		uint8  OpenSwitch;//ϵͳ�ܿ���
		uint8  Condition_OpenSwitch;//�յ�����
		uint8  ConditionMode;//�յ�ģʽ
		uint8  WindSpeed;//����
		uint8  XinFeng_Heat_Flag;//�·����ȱ�־λ
		uint8  XinFeng_Flag;//���ŷ����־λ
		uint8  JingHua_Flag;//������־λ
		uint8  Humidity_Flag;//��ʪ��־λ=1��ʪ��=2��ʪ��=0��״̬��
		uint8  Drain_Pump_Flag;//��ˮ�ñ�־λ
		uint8  State_Change;
	}Set_CMD;      //����ָ��	
	struct 	Parameter0
	{
		uint8 XinFeng_sta;
		uint8 Humidity_sta;
		uint8 Drain_PUMP_sta;
		uint8 JingHua_sta;
		uint8 XinFeng_Heat_sta;
	}Task_sta;//ϵͳ��������ִ�к󣬸�������Ԫ״̬
	struct Parameter1
	{
		uint8  Temperature;
		uint8  Humidity;
		uint8  CO2;
		uint8  PM25;
		uint8  VOC;
	}Set_Parameter;//�ⲿ�������ô���
	struct Parameter2
	{
		uint8 Mode;//���ؿյ���ǰģʽ
		uint8 Temperature_T1;//���ؿյ���ǰT1�¶�
		uint8 Temperature_T2;//���ؿյ���ǰT2�¶�
		uint8 WindSpeed;//���ؿյ���ǰ����
		uint8 Fault_Mode;//���ؿյ�����ģʽ
		uint8 Condition_M10_Sta;
	}Condition_Back;//�յ����ص�����
		
	uint8 System_Fault;//����ϵͳ����ģʽ��¼
	
}APPdata;

void APP_Init(void);
void APP_Run(void);
#ifdef __cplusplus
}
#endif

#endif /* _app_H_ */
