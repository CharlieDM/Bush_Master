#include "app.h"
#include "appdata.h"
#include "io.h"
APPdata  APP_Data;
//计算spi累加和
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
static void Task_Data_Process(void)
{
	if((Data.stAerkate.usOnoffMode==0x01)&&(APP_Data.System_Fault==0))
	{
		APP_Data.Set_CMD.OpenSwitch=1;
		APP_Data.Set_CMD.Condition_OpenSwitch=1;
	}
	else if(APP_Data.System_Fault==1)
	{
		APP_Data.Set_CMD.Condition_OpenSwitch=0;
	}
	else
	{
		APP_Data.Set_CMD.OpenSwitch=0;
		APP_Data.Set_CMD.Condition_OpenSwitch=0;
	}
	if(Data.stAerkate.usHotCoolMode==0x01)
	{
		APP_Data.Set_CMD.ConditionMode=3;//制冷	
	}
	else  if(Data.stAerkate.usHotCoolMode==0x02)
	{
		APP_Data.Set_CMD.ConditionMode=4;//制热
	}
	else  if(Data.stAerkate.usHotCoolMode==0x00)
	{
		APP_Data.Set_CMD.ConditionMode=5;//通风
	}	
	if(Data.stAerkate.usFanSpeed==0x00)
	{
		APP_Data.Set_CMD.WindSpeed=1;
	}
	else if(Data.stAerkate.usFanSpeed==0x01)
	{
		APP_Data.Set_CMD.WindSpeed=2;
	}
	else if(Data.stAerkate.usFanSpeed==0x02)
	{
		APP_Data.Set_CMD.WindSpeed=3;
	}
	//空调温度设定
	APP_Data.Set_Parameter.Temperature=Data.stAerkate.usTempture;;
	if(Data.stAerkate.usXinFeng==0x01)
	{
		APP_Data.Set_CMD.XinFeng_Flag=1;
	}
	else 
	{
		APP_Data.Set_CMD.XinFeng_Flag=0;
	}
	if(Data.stAerkate.usElecHeating==0x01)
	{//暂时不开放
		APP_Data.Set_CMD.XinFeng_Heat_Flag=0;
	}
	else
	{
		APP_Data.Set_CMD.XinFeng_Heat_Flag=0;
	}
	if(Data.stAerkate.usHumidity==0x01)
	{
		if(APP_Data.Set_CMD.ConditionMode==4)//如果是制热模式才能加湿
		{
			APP_Data.Set_CMD.Humidity_Flag=1;//加湿
		}
		
	}
	else if(Data.stAerkate.usHumidity==0x02)
	{
		if(APP_Data.Set_CMD.ConditionMode==3)//制冷模式下才可以除湿
		{
			APP_Data.Set_CMD.Humidity_Flag=2;//除湿
		}
	}
	else if(Data.stAerkate.usHumidity==0x00)
	{
		APP_Data.Set_CMD.Humidity_Flag=0;//不加湿不除湿
	}
}
uint8  rbuf[26];
static void Task_Air_Condition(void)
{
	uint8  i = 0;
	uint8  temp = 0;
	uint8  temp1 = 0;
	uint8  wbuf[26];

	wbuf[0]  = 0xa5;	 
	wbuf[1]  = 0x48;
	wbuf[2]  = 0x0e;
	wbuf[3]  = 0;
	wbuf[4]  = 0x77;  //室温对应的AD值
	if(APP_Data.Set_CMD.WindSpeed==1)
	{
		  temp &=~0x6a;
		  temp |= 0x60;//低风
	}
	else if(APP_Data.Set_CMD.WindSpeed==2)
	{
		  temp &=~0x6a;//中风
		  temp |= 0x40;
	}
	else if(APP_Data.Set_CMD.WindSpeed==3)
	{
		  temp &=~0x6a;//高风
		  temp |= 0x20;
	}
	if(APP_Data.Set_CMD.Condition_OpenSwitch==1)//开机
	{
		 temp=temp+0x02;
	}
	else              
	{
		 temp =temp+0x01;
	}
	wbuf[5]  =temp; 
	//模式设置
	if((APP_Data.Set_CMD.ConditionMode == 1)||(APP_Data.Set_CMD.ConditionMode == 3))
	{
		//制冷模式
		temp &=~0xe0;
		temp |= 0x20;
	}
	else if((APP_Data.Set_CMD.ConditionMode == 2)||(APP_Data.Set_CMD.ConditionMode == 4))
	{
		//制热模式
		temp &=~0xe0;
		temp |= 0x80;
	}	
	else if(((APP_Data.Set_CMD.ConditionMode==5)&&((APP_Data.Set_CMD.XinFeng_Flag==1)||(APP_Data.Set_CMD.JingHua_Flag==1)))||(APP_Data.Set_CMD.ConditionMode ==0))
	{//即不制冷也不制热，且风速不为零，那么改为通风模式,或者是自动净化下的通风就是通风模式
		//通风模式
		temp &=~0xe0;
		temp |= 0x60;
	}
	if((APP_Data.Set_Parameter.Temperature >= 16)&&(APP_Data.Set_Parameter.Temperature <= 32))
	{
		//设定温度
		temp &=~0x1f;
		temp |= (APP_Data.Set_Parameter.Temperature-16);
	}		 			
	wbuf[6]  =temp;  //工作模式 ，设定温度
	wbuf[7]  = 0;   
	wbuf[8]  = 0; 
	wbuf[9]  = 0; 
	wbuf[10] = 0; 
	wbuf[11] = 0; 
	wbuf[12] = 0; 
	wbuf[13] = SPI_CheckSum(wbuf,13); //校验和1		
	wbuf[14] = 0; 
	wbuf[15] = 0; 
	wbuf[16] = 0xff;
	wbuf[17] = 0;
	wbuf[18] = 0; //设定地址参数
	wbuf[19] = 0;
	wbuf[20] = 0;
	wbuf[21] = 0;
	wbuf[22] = 0;
	wbuf[23] = 0;
	wbuf[24] = 0;
	wbuf[25] = (~SPI_CheckSum(wbuf,25))+1; //校验和2
	//判断接收到的帧头是否正确,且开机状态
	Device.Spi.ReadWrite(208,wbuf,rbuf);
	//注意一下去反程序基于外围电路而言的，V0.1版本不需要该取反程序，请注意
	for(i=0;i<26;i++)
	{
		rbuf[i]=~rbuf[i];
	}
	//判断接收到的帧头是否正确,且开机状态
	if((rbuf[0] == 0xa5)&&(APP_Data.Set_CMD.Condition_OpenSwitch))
	{
		temp = (rbuf[7]>>5)&0x07;
		if(temp == 0)
		{
			//自动模式
			APP_Data.Condition_Back.Mode = 0;
		}
		else if(temp == 1)
		{
			//制冷模式
			APP_Data.Condition_Back.Mode = 1;
		}
		else if(temp == 3)
		{
			//通风模式
			APP_Data.Condition_Back.Mode = 3;
		}
		else if(temp == 4)
		{
			//制热模式
			APP_Data.Condition_Back.Mode= 2;
		}
			//防冷风、冷冻功能
		APP_Data.Condition_Back.Condition_M10_Sta	= (rbuf[10]>>3)&0x03;	
		}
		//T1反馈温度值
		 APP_Data.Condition_Back.Temperature_T1=rbuf[5];
		if((rbuf[6]&0x20)&&(rbuf[6]&0x40))
		{
			APP_Data.Condition_Back.WindSpeed=1;//低速
		}	
		else if(rbuf[6]&0x40)
		{
			 APP_Data.Condition_Back.WindSpeed=2;//中速
		}
		else if(rbuf[6]&0x20)
		{
			APP_Data.Condition_Back.WindSpeed=3;//高速
		}			

		if(((rbuf[20]&0x01)==0)&&((rbuf[20]&0x02)==0)&&((rbuf[20]&0x04)==0)&&((rbuf[20]&0x08)==0)&&((rbuf[20]&0x10)==0)&&((rbuf[20]&0x20)==0))
		{//空调报错清除
			APP_Data.Condition_Back.Fault_Mode=0;
		}
		else
		{//空调报错类型
			temp1=0x01;
			for(i=0;i<8;i++)
			{
				if(rbuf[21]&0x02)//byte21
				{
					APP_Data.Condition_Back.Fault_Mode=23;
					break;
				}
				else if(rbuf[20]&temp1)//byte20
				{//空调错误类型，只记录第一个错误类型
					APP_Data.Condition_Back.Fault_Mode=11+i;
					break;
				}
				else if(rbuf[9]==0xe1)//byte9
				{
					//空调错误类型，只记录第一个错误类型
					APP_Data.Condition_Back.Fault_Mode=17;
					break;
				}
				else if(rbuf[9]==0xe2)//byte9
				{
					//空调错误类型，只记录第一个错误类型
					APP_Data.Condition_Back.Fault_Mode=18;
					break;
				}
				else if(rbuf[9]==0xe3)//byte9
				{
					//空调错误类型，只记录第一个错误类型
					APP_Data.Condition_Back.Fault_Mode=19;
					break;
				}
				else if(rbuf[9]==0xe5)//byte9
				{
					//空调错误类型，只记录第一个错误类型
					APP_Data.Condition_Back.Fault_Mode=20;
					break;
				}
				else if(rbuf[9]==0xf2)//byte9
				{
					//空调错误类型，只记录第一个错误类型
					APP_Data.Condition_Back.Fault_Mode=21;
					break;
				}
				else if(rbuf[9]==0xf5)//byte9
				{
					//空调错误类型，只记录第一个错误类型
					APP_Data.Condition_Back.Fault_Mode=22;
					break;
				}
				temp1=temp1<<1;			
			}
		 }
		 Data.stAerkate.usHotCoolMode=APP_Data.Condition_Back.Mode;
		 Data.stAerkate.usResTempture=APP_Data.Condition_Back.Temperature_T1;
		 Data.stAerkate.usFaultMode=APP_Data.Condition_Back.Fault_Mode;
}

static void Task_XinFeng(void)
{
	if(APP_Data.Set_CMD.XinFeng_Flag==1)
	{		
		FRESH_AIR_ON;
		EXHAUST_AIR_ON ;
		//排风阀开
		EXHAUST_AIR_POS_ON;
		EXHAUST_AIR_NEG_OFF;
		//新风阀开
		FRESH_AIR_POS_ON;
		FRESH_AIR_NEG_OFF;
		Data.stAerkate.usXinFeng=1;
		APP_Data.Task_sta.XinFeng_sta=1;
		if(APP_Data.Set_CMD.XinFeng_Heat_Flag==1)
		{
			AIR_ELECTRIC_HEAT_ON ;
			Data.stAerkate.usElecHeating=1;
			APP_Data.Task_sta.XinFeng_Heat_sta=1;
		}
		else
		{
			AIR_ELECTRIC_HEAT_OFF;
			Data.stAerkate.usElecHeating=0;
			APP_Data.Task_sta.XinFeng_Heat_sta=0;
		}
	}
	else 
	{
		FRESH_AIR_OFF;
		EXHAUST_AIR_OFF;
		//排风阀关
		EXHAUST_AIR_POS_OFF;
		EXHAUST_AIR_NEG_ON;
		//新风阀关
		FRESH_AIR_POS_OFF;
		FRESH_AIR_NEG_ON;
		Data.stAerkate.usXinFeng=0;
		APP_Data.Task_sta.XinFeng_sta=0;
	}
}
static void Task_Humidity(void)
{
	static uint16 HUMIDIFIER_NS_Count=0;
	static uint8 HUMIDIFIER_Fault=0;
	uint16   DIN_TEMP=0;
	DIN_TEMP=0;//临时值清零
	DIN_TEMP=ReadIO();
	if(APP_Data.Task_sta.Humidity_sta==1)
	{
		HUMIDIFIER_NS_Count++;
		if(HUMIDIFIER_NS_Count>120)
		{
			APP_Data.Set_CMD.Humidity_Flag=0;
		}
	}
	if((APP_Data.Set_CMD.Humidity_Flag==1)&&((DIN_TEMP&0x0400)==0))
	{	
		HUMIDIFIER_ON;
		DHUMIDIFIER_OFF;
		Data.stAerkate.usHumidity=1;
		APP_Data.Task_sta.Humidity_sta=1;
		HUMIDIFIER_Fault=0;
	}
	else if(APP_Data.Set_CMD.Humidity_Flag==2)
	{
		HUMIDIFIER_OFF;
		DHUMIDIFIER_ON;
		Data.stAerkate.usHumidity=2;
		APP_Data.Task_sta.Humidity_sta=2;
	}
	else if((DIN_TEMP&0x0400)==1)
	{
		HUMIDIFIER_Fault=1;
	}
	else
	{
		HUMIDIFIER_OFF;
		DHUMIDIFIER_OFF;
		Data.stAerkate.usHumidity=0;
		APP_Data.Task_sta.Humidity_sta=0;
	}
	//如果无其他报警故障
	if((Data.stAerkate.usFaultMode<10)&&(HUMIDIFIER_Fault==1))
	{
		Data.stAerkate.usFaultMode=31;//加湿器报警
	}
}
static void Task_Drain_PUMP(void)
{
	static uint16 Last_Humidity = 2;//历史湿度记录，初始化的时候不能使改值位负数、
	static uint8 Drain_PUMP_Fault=0;
	static uint8 Drain_PUMP_NS_Fault_Flag=0;
	static uint8 Drain_PUMP_NS_Flag=0;
	static uint16 Drain_PUMP_NS_Count=0;
	uint16   DIN_TEMP=0;
	//加湿，和提升泵报警输入检测
	if(APP_Data.Task_sta.Drain_PUMP_sta==1)
	{
		Drain_PUMP_NS_Count++;
		if( Drain_PUMP_NS_Count>100)
		{
			Drain_PUMP_NS_Fault_Flag=1;
		}
		if( Drain_PUMP_NS_Count>360)
		{
			Drain_PUMP_NS_Count=0;
			Drain_PUMP_NS_Flag=1;
		}
	}
	else 
	{
		Drain_PUMP_NS_Count=0;
		Drain_PUMP_NS_Fault_Flag=0;
		Drain_PUMP_NS_Flag=0;
	}
	DIN_TEMP=0;//临时值清零
	DIN_TEMP=ReadIO();
	//提升泵	相关程序(排水泵)
	if((((DIN_TEMP&0x0800)==0)&&(Drain_PUMP_Fault==0))||(APP_Data.Task_sta.Drain_PUMP_sta==1))
	{	
		APP_Data.Set_CMD.Drain_Pump_Flag=1;		
		if(APP_Data.Task_sta.Drain_PUMP_sta==1)
		{
		   //提升泵工作100s之后还未断开开关信号，那么
			if(Drain_PUMP_NS_Fault_Flag==1)
			{
				if((DIN_TEMP&0x0800)==0)
				{
					Drain_PUMP_Fault=1;
					//提升泵只在空调制冷状况下才工作
					if((APP_Data.Set_CMD.ConditionMode==1)||(APP_Data.Set_CMD.ConditionMode==3))
					{
						APP_Data.System_Fault |= 0x01;
					}
				}
			}
			//提升泵工作周期180S时间到，此时如果为报错的话，正常情况下浮球开关量已经断开
			if(Drain_PUMP_NS_Flag==1)
			{
				APP_Data.Set_CMD.Drain_Pump_Flag=0;
			}
		}
	}
	else
	{
		APP_Data.Task_sta.Drain_PUMP_sta=0;
		APP_Data.Set_CMD.Drain_Pump_Flag=0;
		if(DIN_TEMP&0x0800)//浮球断开
		{
			APP_Data.System_Fault &= ~(0x01);
			Drain_PUMP_Fault=0;
		}
			
	}		
	//if(APP_Data.Set_CMD.OpenSwitch==1)
	{
		if(APP_Data.Set_CMD.Drain_Pump_Flag==1)
		{
			DRAIN_PUMP_ON;
			APP_Data.Task_sta.Drain_PUMP_sta=1;
		}
		else
		{
			DRAIN_PUMP_OFF;
			APP_Data.Task_sta.Drain_PUMP_sta=0;
		}
    }
	//如果无其他报警故障
	if((Data.stAerkate.usFaultMode<10)&&(Drain_PUMP_Fault==1))
	{
		Data.stAerkate.usFaultMode=33;//系统提升泵故障
	}
}
static void Task_Close(void)
{
	APP_Data.Set_CMD.Condition_OpenSwitch=0;//关闭空调，返回状态是无
	FRESH_AIR_OFF;
	FRESH_AIR_POS_OFF;
	FRESH_AIR_NEG_OFF;
	EXHAUST_AIR_OFF;
	EXHAUST_AIR_NEG_OFF;
	EXHAUST_AIR_POS_OFF;

	APP_Data.Task_sta.XinFeng_sta=0;
	Data.stAerkate.usXinFeng=0;
	HUMIDIFIER_OFF;
	DHUMIDIFIER_OFF;
	APP_Data.Task_sta.Humidity_sta=0;
	Data.stAerkate.usHumidity=0;
}
void APP_Init(void)
{
	//Parameter Init
	APP_Data.System_Fault=0;
	Device.Systick.Register(100,Task_Drain_PUMP);//水泵执行处理函数
	Device.Systick.Register(100,Task_Humidity);//加湿器执行函数
	Device.Systick.Register(250,Task_Air_Condition);
}
void APP_Run(void)
{
	Task_Data_Process();
	if(APP_Data.Set_CMD.OpenSwitch==1)
	{
		Task_XinFeng();
		Task_Humidity();
	}
	else
	{
        Task_Close();
	}
}