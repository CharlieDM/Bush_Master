#include "app.h"
#include "data.h"
#include "io.h"
APPdata  APP_Data;
APPtask  APP_Task;
Dataall  Data_All ;

//����spi�ۼӺ�
static uint8 SPI_CheckSum(uint8 *buf,uint8 cnt)
{
	uint8  i = 0;
	uint16 sum = 0;
	for(i=0;i<cnt;i++)
	{
			sum += buf[i];
    }
	return sum;
}
static void Task_Air_Condition(void)
{
	uint8  i = 0;
	uint8  temp = 0;
	uint8  temp1 = 0;
	uint8  wbuf[26];
	uint8  rbuf[26];	

	wbuf[0]  = 0xa5;	 
	wbuf[1]  = 0x48;
	wbuf[2]  = 0x0e;
	wbuf[3]  = 0;
	wbuf[4]  = 0x77;  //���¶�Ӧ��ADֵ
	if(APP_Data.Set_CMD.WindSpeed==1)
	{
		  temp &=~0x6a;
		  temp |= 0x60;//�ͷ�
	}
	else if(APP_Data.Set_CMD.WindSpeed==2)
	{
		  temp &=~0x6a;//�з�
		  temp |= 0x40;
	}
	else if(APP_Data.Set_CMD.WindSpeed==3)
	{
		  temp &=~0x6a;//�߷�
		  temp |= 0x20;
	}
	if(APP_Data.Set_CMD.WindSpeed==1)//����
	{
		 temp=temp+0x02;
	}
	else              
	{
		 temp =temp+0x01;
	}
	wbuf[5]  =temp; 
	//ģʽ����
	if((APP_Data.Set_CMD.ConditionMode == 1)||(APP_Data.Set_CMD.ConditionMode == 3))
	{
		//����ģʽ
		temp &=~0xe0;
		temp |= 0x20;
	}
	else if((APP_Data.Set_CMD.ConditionMode == 2)||(APP_Data.Set_CMD.ConditionMode == 4))
	{
		//����ģʽ
		temp &=~0xe0;
		temp |= 0x80;
	}	
	else if(((APP_Data.Set_CMD.ConditionMode==5)&&((APP_Data.Set_CMD.XinFeng_Flag==1)||(APP_Data.Set_CMD.JingHua_Flag==1)))||(APP_Data.Set_CMD.ConditionMode ==0))
	{//��������Ҳ�����ȣ��ҷ��ٲ�Ϊ�㣬��ô��Ϊͨ��ģʽ,�������Զ��廪�µ�ͨ�����ͨ��ģʽ
		//ͨ��ģʽ
		temp &=~0xe0;
		temp |= 0x60;
	}
	if((APP_Data.Set_Parameter.Temperature >= 16)&&(APP_Data.Set_Parameter.Temperature <= 32))
	{
		//�趨�¶�
		temp &=~0x1f;
		temp |= (APP_Data.Set_Parameter.Temperature-16);
	}		 			
	wbuf[6]  =temp;  //����ģʽ ���趨�¶�
	wbuf[7]  = 0;   
	wbuf[8]  = 0; 
	wbuf[9]  = 0; 
	wbuf[10] = 0; 
	wbuf[11] = 0; 
	wbuf[12] = 0; 
	wbuf[13] = SPI_CheckSum(wbuf,13); //У���1		
	wbuf[14] = 0; 
	wbuf[15] = 0; 
	wbuf[16] = 0xff;
	wbuf[17] = 0;
	wbuf[18] = 0; //�趨��ַ����
	wbuf[19] = 0;
	wbuf[20] = 0;
	wbuf[21] = 0;
	wbuf[22] = 0;
	wbuf[23] = 0;
	wbuf[24] = 0;
	wbuf[25] = (~SPI_CheckSum(wbuf,25))+1; //У���2
	//�жϽ��յ���֡ͷ�Ƿ���ȷ,�ҿ���״̬
	Device.Spi.ReadWrite(208,wbuf,rbuf);
	//ע��һ��ȥ�����������Χ��·���Եģ�V0.1�汾����Ҫ��ȡ��������ע��
	for(i=0;i<26;i++)
	{
		rbuf[i]=~rbuf[i];
	}
	//�жϽ��յ���֡ͷ�Ƿ���ȷ,�ҿ���״̬
	if((rbuf[0] == 0xa5)&&(APP_Data.Set_CMD.Condition_OpenSwitch))
	{
		temp = (rbuf[7]>>5)&0x07;
		if(temp == 0)
		{
			//�Զ�ģʽ
			APP_Data.Condition_Back.Mode = 0;
		}
		else if(temp == 1)
		{
			//����ģʽ
			APP_Data.Condition_Back.Mode = 1;
		}
		else if(temp == 3)
		{
			//ͨ��ģʽ
			APP_Data.Condition_Back.Mode = 3;
		}
		else if(temp == 4)
		{
			//����ģʽ
			APP_Data.Condition_Back.Mode= 2;
		}
			//����硢�䶳����
		APP_Data.Condition_Back.Condition_M10_Sta	= (rbuf[10]>>3)&0x03;	
		}
		//T1�����¶�ֵ
		 APP_Data.Condition_Back.Temperature_T1=rbuf[5];

		if((rbuf[6]&0x20)&&(rbuf[6]&0x40))
		{
			APP_Data.Condition_Back.WindSpeed=1;//����
		}	
		else if(rbuf[6]&0x40)
		{
			 APP_Data.Condition_Back.WindSpeed=2;//����
		}
		else if(rbuf[6]&0x20)
		{
			APP_Data.Condition_Back.WindSpeed=3;//����
		}			

		if(((rbuf[20]&0x01)==0)&&((rbuf[20]&0x02)==0)&&((rbuf[20]&0x04)==0)&&((rbuf[20]&0x08)==0)&&((rbuf[20]&0x10)==0)&&((rbuf[20]&0x20)==0))
		{//�յ��������
			APP_Data.Condition_Back.Fault_Mode=0;
		}
		else
		{//�յ���������
			temp1=0x01;
			for(i=0;i<8;i++)
			{
				if(rbuf[20]&temp1)
				{//�յ��������ͣ�ֻ��¼��һ����������
					APP_Data.Condition_Back.Fault_Mode=11+i;
					break;
				}
				temp1=temp1<<1;			
			}
		}

}
static void Task_Data_Process(void)
{
	if(Data_All.Mode==Host_Mode)
	{
		
	}
	else if(Data_All.Mode==Slave_Mode)
	{
		if(Data_All.Slave_Data.StatusH&0x04)
		{
			APP_Data.Set_CMD.OpenSwitch=1;
		}
		else
		{
			APP_Data.Set_CMD.OpenSwitch=0;
		}
		if(Data_All.Slave_Data.StatusH&0x02)
		{
			APP_Data.Set_CMD.ConditionMode=0;
		}
		else 
		{
			APP_Data.Set_CMD.ConditionMode=5;
		}
		if(Data_All.Slave_Data.StatusH&0x01)
		{
			APP_Data.Set_CMD.XinFeng_Heat_Flag=1;
		}
		else 
		{
			APP_Data.Set_CMD.XinFeng_Heat_Flag=0;
		}
		if(((Data_All.Slave_Data.StatusL&0x80)==1)&&((Data_All.Slave_Data.StatusL&0x40)==0))
		{
			APP_Data.Set_CMD.ConditionMode=3;
		}
		else if(((Data_All.Slave_Data.StatusL&0x80)==0)&&((Data_All.Slave_Data.StatusL&0x40)==1))
		{
			APP_Data.Set_CMD.ConditionMode=4;
		}
		else
		{
			APP_Data.Set_CMD.ConditionMode=5;
		}
		if(((Data_All.Slave_Data.StatusL&0x20)==1)&&((Data_All.Slave_Data.StatusL&0x10)==1))
		{
			APP_Data.Set_CMD.XinFeng_Flag=1;
		}
		else
		{
			APP_Data.Set_CMD.XinFeng_Flag=0;
		}
		if(((Data_All.Slave_Data.StatusL&0x08)==1)&&((Data_All.Slave_Data.StatusL&0x04)==0))
		{
			APP_Data.Set_CMD.Humidity_Flag=1;
		}
		else if(((Data_All.Slave_Data.StatusL&0x08)==0)&&((Data_All.Slave_Data.StatusL&0x04)==1))
		{
			APP_Data.Set_CMD.Humidity_Flag=2;
		}
		else
		{
			APP_Data.Set_CMD.XinFeng_Flag=0;
		}
		if(((Data_All.Slave_Data.StatusL&0x02)==1)&&((Data_All.Slave_Data.StatusL&0x01)==1))
		{
			APP_Data.Set_CMD.WindSpeed=1;
		}
		else if(((Data_All.Slave_Data.StatusL&0x02)==1)&&((Data_All.Slave_Data.StatusL&0x01)==0))
		{
			APP_Data.Set_CMD.WindSpeed=2;
		}
		else if(((Data_All.Slave_Data.StatusL&0x02)==0)&&((Data_All.Slave_Data.StatusL&0x01)==1))
		{
			APP_Data.Set_CMD.WindSpeed=3;
		}

	}
	else if(Data_All.Mode==GUI_Mode)
	{
		
	}
}
static void Task_XinFeng(void)
{
	if(APP_Data.Set_CMD.XinFeng_Flag==1)
	{
		APP_Data.Task_sta.XinFeng_sta=1;
		if(APP_Data.Set_CMD.XinFeng_Heat_Flag==1)
		{
			FRESH_AIR_ON;
			EXHAUST_AIR_ON ;
			APP_Data.Task_sta.XinFeng_Heat_sta=1;
		}
		else
		{
			FRESH_AIR_OFF;
			EXHAUST_AIR_OFF;
			APP_Data.Task_sta.XinFeng_Heat_sta=0;
		}
	}
	else 
	{
		FRESH_AIR_OFF;
		EXHAUST_AIR_OFF;
		APP_Data.Task_sta.XinFeng_sta=0;
	}
}
static void Task_Humidity(void)
{
	if(APP_Data.Set_CMD.Humidity_Flag==1)
	{
		HUMIDIFIER_ON;
		RESERVED2_OFF;//���ö�Ϊ��ʪ
		APP_Data.Task_sta.Humidity_sta=1;
	}
	else if(APP_Data.Set_CMD.Humidity_Flag==2)
	{
		HUMIDIFIER_OFF;
		RESERVED2_ON;
		APP_Data.Task_sta.Humidity_sta=2;
	}
	else
	{
		HUMIDIFIER_OFF;
		RESERVED2_OFF;
		APP_Data.Task_sta.Humidity_sta=0;
	}
}
static void Task_Drain_PUMP(void)
{
	if(APP_Data.Set_CMD.Drain_Pump_Flag==1)
	{
		//DRAIN_PUMP_ON;
		RESERVED1_ON;
		APP_Data.Task_sta.Drain_PUMP_sta=1;
	}
	else
	{
		//DRAIN_PUMP_OFF;
		RESERVED1_OFF;
		APP_Data.Task_sta.Drain_PUMP_sta=0;
	}
}
void APP_init(void)
{
	APP_Task.Data_Process=Task_Data_Process;
	APP_Task.Air_Condition=Task_Air_Condition;
	APP_Task.Drain_PUMP =Task_Drain_PUMP;
	APP_Task.XinFeng=Task_XinFeng;
	APP_Task.Humidity=Task_Humidity;
}