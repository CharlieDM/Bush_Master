#include "bsp_global.h"
#include "app.h"
#include "spi.h"
#include "tempature.h"
#include "ads1232.h"
#include "comm.h"
//开机空调延时30S标志位
uint8_t	TIMER_OPEN_DELAY_Flag=0;
//开机空调延时30S计数
uint16_t	TIMER_OPEN_DELAY_Count=0;
//净化状态，0：关闭；1：打开；
uint8_t  JingHua_Sta = 0;
//强制净化，0：关闭；1：打开；
uint8_t  JingHua_Force = 0;
//新风状态，0：关闭；1：打开；
uint8_t  XinFeng_Sta = 0;
//强制新风，0：关闭；1：打开；
uint8_t  XinFeng_Force = 0;
//当前空调模式状态,0：自动；1：制冷；2：制热;
uint8_t  Current_ConditionMode_Sta = 0;
//当前过滤清洁,1:红；2：黄；3：绿；
uint8_t  Current_FilterClean = 0;
//当前风速,1：低速；2：中速;3:高速；0:关闭
uint8_t  Current_WindSpeed = 0;
//设置的温度没有负数
uint16_t Set_Temperature = 22;
//设置湿度
uint16_t Set_Humidity =50;
//设置二氧化碳浓度
uint16_t Set_CO2 = 1000;
//设置PM2.5浓度
uint16_t Set_PM25 =25;
//设置VOC浓度
uint16_t Set_VOC = 25;
//设置风速,1：低速；2：中速;3:高速；0:关闭
uint8_t  Set_WindSpeed = 1;
//设置工作模式，0:空调模式；1：居家模式;2:离家模式；3：空气处理模式；
uint8_t  Set_WorkMode = 0;
//控制启动开关，0：关闭；1：打开,包括内机和外机
uint8_t  Set_OpenSwitch = 0;
//空调外机启动开关 0：关闭；1：打开
uint8_t  Set_Condition_OpenSwitch = 0;
//空调外机启动开关状态 0：关闭；1：打开
uint8_t  Set_Condition_OpenSwitch_sta=0;
//超强制热状态
uint8_t  Heat_Sta = 0;
//自动模式下的预加热功能
uint8_t  Pre_Heat_Flag = 0;
uint8_t  Pre_Heat_Count = 0;
//自动模式下超强预加热等待标志位
uint8_t  Heat_Wait_Flag = 0;
uint32_t Heat_Wait_Count = 0;
//手动模式下的超强制热标志位和计数
uint32_t Set_HeatSwitch_Count=0;
uint8_t  Set_HeatSwitch_sta = 0;
//超强制热,0：关闭，1：打开；
uint8_t  Set_HeatSwitch = 0;//超强制热的总开关
//空调模式切换，0：自动，1：制冷,2:制热,3:手动模式下的制冷，4：手动模式下的制热,5:手动情况下无操作
uint8_t  Set_ConditionMode = 0;
//各个房间的温度，共5个，第5个房间显示在主界面
int16_t Room_Temperature[5];//温度有负数
uint8_t Room_Temperature_zhengfu[5];//温度有负数
//各个房间的湿度，共5个,第5个房间显示在主界面
uint16_t Room_Humidity[5];
//各个房间的PM2.5，共5个,第5个房间显示在主界面
uint16_t Room_PM25[5];
//各个房间的CO2，共5个,第5个房间显示在主界面
uint16_t Room_CO2[5];
//各个房间的VOC，共5个,第5个房间显示在主界面
uint16_t Room_VOC[5];
//各个房间送风机运行状态反馈,0：关闭；1：打开；
uint8_t  Room_Wind_Feedback[5];
//各个房间的工作状态
uint8_t  Room_Work_State[5];
//风阀开启关闭命令,主要是客厅的风阀开关
uint8_t  Room_Wind_Command[5];
//防冷风、除霜功能
uint8_t  Condition_M10_Sta = 0; 
//报错码
uint8_t  Fault_Mode=0;

uint8_t  CO2_VOC_Over_cnt=0;//参数超标技术
uint8_t  CO2_VOC_Over_flag=0;//参数超标标志位
uint8_t  PM25_Over_cnt=0;//参数超标技术
uint8_t  PM25_Over_flag=0;//参数超标标志位

uint8_t  State0_Flag = 0;//分控器工作状态
uint8_t  State0_Count = 0;//工作状态相关计数
uint16_t State0_TIMER_Count = 0;//工作状态相关计数



int8_t  Temperature_T1=0;
/*
*********************************************************************************************************
*	函 数 名: Init_Hard
*	功能说明: 初始化硬件
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Init_Hard(void)
{
	//初始化输入
	bsp_InitDin();
	//初始化输出
	bsp_InitDout();
	//初始化定时器
	bsp_InitTimer();
	//初始化串口
	bsp_InitUart();
	//初始化SPI
	SPI1_Init();
}
/*
*********************************************************************************************************
*	函 数 名: Init_Soft
*	功能说明: 初始化软件
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Init_Soft(void)
{
	uint8_t  i=0;
	uint16_t buf[45];
	for(i=0;i<15;i++)
	{
		bsp_DoutOff(i);
  }
	memset(buf,0,40);
	//读取flash中的数据
	bsp_ReadCpuFlash(PARA_SAVE_ADDR, (uint8_t*)buf, 44);
	//判断是否是出厂上电
//	if((buf[0] != 0xffff)&&(buf[0] != 0))
//	{
//		//5房间环境参数
//		
//		//g_Pdu.BitCoil[0] = buf[42];
//		g_Pdu.BitCoil[1] = buf[43];
//  }
		for(i=0;i<44;i++)
		{
			g_Pdu.RegHold[i]  =0;// buf[i];
    }
	  TIMER_OPEN_DELAY_Flag=0;//开机延时30S标志位清除
		TIMER_OPEN_DELAY_Count=0;//开机延时30S计数
		Set_Temperature=22;//默认开机有初始值
		Set_PM25=50;
		Set_CO2=1000;
		Set_VOC=25;
		Set_WindSpeed = 1;	    //再次开机保持最低速
		JingHua_Force=0;
		XinFeng_Force=0;
		Room_Wind_Command[0]=0;//客厅风阀关闭命令，0状态不给出命令
		Set_ConditionMode=0;//空调模式
		Set_Condition_OpenSwitch=0;//空调外机启动开关
		Set_Condition_OpenSwitch_sta=0;//空调外机启动状态
		g_Pdu.BitCoil[0]|=0x20;//初始启动默认是自动模式
	  g_Pdu.BitCoil[0] &=~0x07;//低风状态位
	  g_Pdu.BitCoil[0] |= 0x01;
	
	//默认过滤清洁,绿色
	Current_FilterClean = 3;
	//没有故障发生初始化
	Fault_Mode=0;
	//串口3处在空闲状态
	g_tUart3.Txflag=0x01;
}
/*
*********************************************************************************************************
*	函 数 名: SET_Button_State
*	功能说明: 触摸屏按键状态设置，弹起按下状态设置
*	形    参: _
*	返 回 值: 无
*********************************************************************************************************
*/
void SET_Button_State(uint8_t page,uint8_t addr,uint8_t state)
{
		 UART3TxBuf[0]=0xee;
		 UART3TxBuf[1]=0xb1;
		 UART3TxBuf[2]=0x10;
		 UART3TxBuf[3]=0x00;
		 UART3TxBuf[4]=page;
		 UART3TxBuf[5]=0x00;
		 UART3TxBuf[6]=addr;   
		 UART3TxBuf[7]=state;  
		 UART3TxBuf[8]=0xff;
		 UART3TxBuf[9]=0xfc;
		 UART3TxBuf[10]=0xff;
		 UART3TxBuf[11]=0xff;
	   g_tUart3.Txflag    =0x00;//串口3正在发送数据，请勿更新数据
		 comSendBuf(COM3,UART3TxBuf,12);
		 g_tUart3.Txflag    =0x01;//串口3发送数据完成
}
/*
*********************************************************************************************************
*	函 数 名: SET_JinDU
*	功能说明: 设置进度条数值
*	形    参: _
*	返 回 值: 无
*********************************************************************************************************
*/
void SET_JinDU(uint8_t DAT)
{
		 UART3TxBuf[0]=0xee;
		 UART3TxBuf[1]=0xb1;
		 UART3TxBuf[2]=0x10;
		 UART3TxBuf[3]=0x00;
		 UART3TxBuf[4]=0x01;
		 UART3TxBuf[5]=0x00;
		 UART3TxBuf[6]=0x11;   //按下制冷
		 UART3TxBuf[7]=0x00;  
	   UART3TxBuf[8]=0x00;  
	   UART3TxBuf[9]=0x00;  
	   UART3TxBuf[10]=DAT;  
		 UART3TxBuf[11]=0xff;
		 UART3TxBuf[12]=0xfc;
		 UART3TxBuf[13]=0xff;
		 UART3TxBuf[14]=0xff;
	   g_tUart3.Txflag    =0x00;//串口3正在发送数据，请勿更新数据
		 comSendBuf(COM3,UART3TxBuf,15);
	   g_tUart3.Txflag    =0x01;//串口3发送数据完成
}
/*
*********************************************************************************************************
*	函 数 名: Uart_SendProcess
*	功能说明: 发送数据处理，最要针对屏的数据显示和485读命令下发
*	形    参: _
*	返 回 值: 无
*********************************************************************************************************
*/
void Uart_SendProcess(void)
{
	//主要上传首页传感器数据，工作状态和舒适度，故障代码，空间状态
	uint8_t  i=0;
	static uint8_t  Text0_Flag = 0; //第一页传感器值更新
	static uint8_t  Text1_Flag = 0; //第二页传感器检测值更新
	  
	static uint8_t  State1_Flag = 0;//自动模式情况下的控制按键的状态显示
	static uint8_t  Feel_Flag = 0;  //舒适度
	static uint8_t  Fault_Flag = 0; //错误更新上传标志位
	static uint8_t  T_485_Cmd_ONOFF_Flag = 0;//发送485开关机标志位
	static uint8_t  T_485_Read_Flag = 0;  //读传感器数据操作
	static uint8_t  RT_Condition_Flag =1;//对空调SPI读写操作标志位
	static uint8_t  Fault_Addr_Count = 0; //报警错误计数
  static uint8_t  Feel_Count = 0;       //舒适度相关计数
	
	static uint8_t  State1_Count = 0;      //工作状态相关计数
  static uint8_t  Room_Read_addr=0;     //房间地址数读数据地址
	static uint8_t  Room_Cmd_addr=0;     //房间地址数读数据地址
	static uint8_t  Room_Cmd_addr_Count=0;//多次给风控器下发命令
	       uint8_t  target_temp=0;        //传感器，分控器目标操作选择
	static uint8_t  Set_OpenSwitch_temp=0;
	static uint8_t  Set_OpenSwitch_flag=0;//读取数据和发送命令切换标志位,只能先切换开关状态
	       uint8_t  WORK_Count=0;
	//Text0_Flag=1;
	//Text1_Flag=1;	
	//State0_Flag=1;
  //State1_Flag=1;
	//Feel_Flag=1;
	//T_485_Cmd_Flag=1;
	//T_485_Read_Flag=1;
	//T_485_Cmd_ONOFF_Flag=1;
	//Fault_Flag=1;
	//RT_Condition_Flag=1;
	/*************读分控器外加户外传感器数据**************/
		if(Set_OpenSwitch_flag==1)
		{
				if(T_485_Read_Flag ==0)
				{
						T_485_Read_Flag =1;
						bsp_StartTimer(TIMER_Read_485, 200);//2S 轮询是个房间的分控器，外加一个户外传感器，最低间隔也最好设定在2S
						Room_Read_addr++;
						
					  if(Room_Read_addr<5)
						{
							target_temp=0x02;//操作对象是分控器
							Room_Board_Write(target_temp,Room_Read_addr,0x02);	
						}	
            else if(Room_Read_addr==5)
						{
							target_temp=0x01;//操作对象是传感器模块
							Room_Board_Write(target_temp,0x01,0x02);		
						}	
						else if(Room_Read_addr==5)
						{
							if((Room_Wind_Command[0]==1)&&((Set_ConditionMode ==0)||(Set_ConditionMode ==1)||(Set_ConditionMode ==2)
								||(Set_ConditionMode ==3)||(Set_ConditionMode ==4)||(Set_ConditionMode ==5))&&(Set_Condition_OpenSwitch==0))
							{
								Room_Board_Write(0x02,0x01,0x03);	//开客厅风阀
							}
						}
				}
				else if(bsp_CheckTimer(TIMER_Read_485))	   
				{
						T_485_Read_Flag =0;
						bsp_StopTimer(TIMER_Read_485);	
						if(Room_Read_addr==6)
						{
							 Room_Read_addr=0;
						}					
				}		
		}
	/*************给分控器，传感器操作下达命令这个只有是状态改变的时候才执行*************/
		if(Set_OpenSwitch_temp!=Set_OpenSwitch)
		{
			 Set_OpenSwitch_flag=0;//正在发送关机指令 
			 if(T_485_Cmd_ONOFF_Flag ==0)
			 {
					T_485_Cmd_ONOFF_Flag=1;
					bsp_StartTimer(TIMER_Cmd_485,200);//状态改变的时候隔2S发送给不同的分控器开关信号
					Room_Cmd_addr++;                  //地址改变		
					if(Set_OpenSwitch==0)
					{
							Room_Board_Write(0x02,Room_Cmd_addr,0x05);//都是读操作Room_Cmd_addr
							g_Pdu.BitCoil[0]&=~0x20;
							g_Pdu.BitCoil[0]|=0x20;//关机之后置位，能够为下一次开机进入自动模式做标志	
					}

			 }
			 else if(bsp_CheckTimer(TIMER_Cmd_485))	   
			 {
						T_485_Cmd_ONOFF_Flag =0;
						bsp_StopTimer(TIMER_Cmd_485);
				   //如果没有进行状态切换是会进入这个条件，然后直接发送读数据发送命令
						if(Room_Cmd_addr==4)              
						{
								Room_Cmd_addr=0;
								Room_Cmd_addr_Count++;
								if(Room_Cmd_addr_Count==2)       //下发开关机命令，发两次，防止数据丢失
								{
									Room_Cmd_addr_Count=0;
									Set_OpenSwitch_temp=Set_OpenSwitch;
									
									if(Set_OpenSwitch==1)
									{
										while((g_tUart3.Txflag==0)||(g_tUart5.Txflag==0));//等待发送完成后，发送下列数据
										UART3TxBuf[0]=0xee;
										UART3TxBuf[1]=0xb1;
										UART3TxBuf[2]=0x10;
										UART3TxBuf[3]=0x00;
										UART3TxBuf[4]=0x00;
										UART3TxBuf[5]=0x00;
										UART3TxBuf[6]=0x20;
										UART3TxBuf[7]=0xb9;//已开机，显示“关”
										UART3TxBuf[8]=0xd8;
										UART3TxBuf[9]=0xff;
										UART3TxBuf[10]=0xfc;
										UART3TxBuf[11]=0xff;
										UART3TxBuf[12]=0xff;
										g_tUart3.Txflag     =0x00;//串口3正在发送数据，请勿更新数据,分开发送归发送，缓存归缓存，后期还是需要分开的
										comSendBuf(COM3,UART3TxBuf,13);
										while((g_tUart3.Txflag==0)||(g_tUart5.Txflag==0));//等待发送完成后，发送下列数据
										UART3TxBuf[0]=0xee;
										UART3TxBuf[1]=0xb1;
										UART3TxBuf[2]=0x10;
										UART3TxBuf[3]=0x00;
										UART3TxBuf[4]=0x01;
										UART3TxBuf[5]=0x00;
										UART3TxBuf[6]=0x02;
										UART3TxBuf[7]=0xb9;//已开机，显示“关”
										UART3TxBuf[8]=0xd8;
										UART3TxBuf[9]=0xff;
										UART3TxBuf[10]=0xfc;
										UART3TxBuf[11]=0xff;
										UART3TxBuf[12]=0xff;
										g_tUart3.Txflag     =0x00;//串口3正在发送数据，请勿更新数据,分开发送归发送，缓存归缓存，后期还是需要分开的
										comSendBuf(COM3,UART3TxBuf,13);
						    	}
									else
									{
										 while((g_tUart3.Txflag==0)||(g_tUart5.Txflag==0));
											UART3TxBuf[0]=0xee;
											UART3TxBuf[1]=0xb1;
											UART3TxBuf[2]=0x10;
											UART3TxBuf[3]=0x00;
											UART3TxBuf[4]=0x00;
											UART3TxBuf[5]=0x00;
											UART3TxBuf[6]=0x20;
											UART3TxBuf[7]=0xbf;//已关机，显示“开”
											UART3TxBuf[8]=0xaa;
											UART3TxBuf[9]=0xff;
											UART3TxBuf[10]=0xfc;
											UART3TxBuf[11]=0xff;
											UART3TxBuf[12]=0xff;
											g_tUart3.Txflag     =0x00;//串口3正在发送数据，请勿更新数据,分开发送归发送，缓存归缓存，后期还是需要分开的
											comSendBuf(COM3,UART3TxBuf,13);
										  while((g_tUart3.Txflag==0)||(g_tUart5.Txflag==0));
											UART3TxBuf[0]=0xee;
											UART3TxBuf[1]=0xb1;
											UART3TxBuf[2]=0x10;
											UART3TxBuf[3]=0x00;
											UART3TxBuf[4]=0x01;
											UART3TxBuf[5]=0x00;
											UART3TxBuf[6]=0x02;
											UART3TxBuf[7]=0xbf;//已关机，显示“开”
											UART3TxBuf[8]=0xaa;
											UART3TxBuf[9]=0xff;
											UART3TxBuf[10]=0xfc;
											UART3TxBuf[11]=0xff;
											UART3TxBuf[12]=0xff;
											g_tUart3.Txflag     =0x00;//串口3正在发送数据，请勿更新数据,分开发送归发送，缓存归缓存，后期还是需要分开的
											comSendBuf(COM3,UART3TxBuf,13);
									}
								} 
					  }  			
				}
		 } 
    else if(Set_OpenSwitch_temp==Set_OpenSwitch)
				       Set_OpenSwitch_flag=1;         //下达命令完成标志位,可以发送读取命令了	
	  if(Set_OpenSwitch==1)
	  { 
			/********************屏幕首页，传感器数值更新**********************/
			if(Text0_Flag==0)//17s更新一次
			{
					Text0_Flag=1;
					//if(g_tUart3.Txflag==0x01)//是否串口3正在空闲状态
					//{	
						//17S更新屏幕一次
						bsp_StartTimer(TIMER_Screen0_Text_Update,1700);
						UART3TxBuf[0]=0xee;
						UART3TxBuf[1]=0xb1;
						UART3TxBuf[2]=0x12;
						UART3TxBuf[3]=0x00;
						UART3TxBuf[4]=0x00;
						for(i=0;i<25;i++)
							{
								/********控件地址 ******/
								UART3TxBuf[5+i*8]=0x00;
								UART3TxBuf[6+i*8]=i+1;
								if(i%5==0)//温度值显示
								{
									 /********文本长度 *****/
										UART3TxBuf[7+i*8]=0x00;
										UART3TxBuf[8+i*8]=0x04;
									 /********文本内容*****/
										UART3TxBuf[9+i*8]=0x20;
									  if(Room_Temperature_zhengfu[i/5]==0x00)
										   UART3TxBuf[10+i*8]=0x20;//Temp_ge;
										else if(Room_Temperature_zhengfu[i/5]==0x80)
											 UART3TxBuf[10+i*8]=0x2d;//Temp_ge;
										UART3TxBuf[11+i*8]=0x30+Room_Temperature[i/5]/10;//Temp_ge;
										UART3TxBuf[12+i*8]=0x30+Room_Temperature[i/5]%10;//Temp_ge;
								}
								else if(i%5==1)//湿度值显示
								{
									 /********文本长度 *****/
										UART3TxBuf[7+i*8]=0x00;
										UART3TxBuf[8+i*8]=0x04;
									 /********文本内容*****/
										UART3TxBuf[9+i*8]=0x20;
										UART3TxBuf[10+i*8]=0x20;//Temp_ge;
										UART3TxBuf[11+i*8]=0x30+Room_Humidity[i/5]/10;//Temp_ge;
										UART3TxBuf[12+i*8]=0x30+Room_Humidity[i/5]%10;//Temp_ge;
								}
								else if(i%5==2)//PM2.5数值显示
								{
									 /********文本长度 *****/
										UART3TxBuf[7+i*8]=0x00;
										UART3TxBuf[8+i*8]=0x04;
									 /********文本内容*****/
										UART3TxBuf[9+i*8]=0x20;
										UART3TxBuf[10+i*8]=0x30+Room_PM25[i/5]/100;
										UART3TxBuf[11+i*8]=0x30+(Room_PM25[i/5]/10)%10;//Temp_ge;
										UART3TxBuf[12+i*8]=0x30+Room_PM25[i/5]%10;//Temp_ge;
								}
								else if(i%5==3)//CO2数值显示
								{
									 /********文本长度 *****/
										UART3TxBuf[7+i*8]=0x00;
										UART3TxBuf[8+i*8]=0x04;
									 /********文本内容*****/
										UART3TxBuf[9+i*8]=0x30+Room_CO2[i/5]/1000;
										UART3TxBuf[10+i*8]=0x30+(Room_CO2[i/5]/100)%10;
										UART3TxBuf[11+i*8]=0x30+(Room_CO2[i/5]/10)%10;//Temp_ge;
										UART3TxBuf[12+i*8]=0x30+Room_CO2[i/5]%10;//Temp_ge;
								}
								else if(i%5==4)
								{
									 /********文本长度 *****/
										UART3TxBuf[7+i*8]=0x00;
										UART3TxBuf[8+i*8]=0x04;
									 /********文本内容*****/
										UART3TxBuf[9+i*8]= 0x20;
										UART3TxBuf[10+i*8]=0x20;
										UART3TxBuf[11+i*8]=0x30+(Room_VOC[i/5]/10)%10;//Temp_ge;
										UART3TxBuf[12+i*8]=0x30+Room_VOC[i/5]%10;//Temp_ge;
								}
							}
							UART3TxBuf[205]=0xff;
							UART3TxBuf[206]=0xfc;
							UART3TxBuf[207]=0xff;
							UART3TxBuf[208]=0xff;
							g_tUart3.Txflag     =0x00;//串口3正在发送数据，请勿更新数据,分开发送归发送，缓存归缓存，后期还是需要分开的
							comSendBuf(COM3,UART3TxBuf,209);
						  g_tUart3.Txflag    =0x01;//串口3发送数据完成
				//	}
			}
			else if(bsp_CheckTimer(TIMER_Screen0_Text_Update))	  
			{     
						Text0_Flag=0;
						bsp_StopTimer(TIMER_Screen0_Text_Update);
			}
				/***************屏幕第二页的传感器监测值更新************************/
			if(Text1_Flag==0)//9s更新一次
			{
					Text1_Flag=1;
					bsp_StartTimer(TIMER_Screen1_Text_Update,900);
					//if(g_tUart3.Txflag==0x01)//是否串口3正在空闲状态
					//{
						UART3TxBuf[0]=0xee;
						UART3TxBuf[1]=0xb1;
						UART3TxBuf[2]=0x12;
						UART3TxBuf[3]=0x00;
						UART3TxBuf[4]=0x01;
						for(i=0;i<4;i++)//4个传感器的检测值
						{
							/********控件地址 ******/
							UART3TxBuf[5+i*8]=0x00;
							UART3TxBuf[6+i*8]=i+24;			
							if(i%4==0)//温度值显示
							{
								 /********文本长度 *****/
									UART3TxBuf[7+i*8]=0x00;
									UART3TxBuf[8+i*8]=0x04;
								 /********文本内容*****/
									UART3TxBuf[9+i*8]=0x20;
								 if(Room_Temperature_zhengfu[0]==0x00)
										   UART3TxBuf[10+i*8]=0x20;//Temp_ge;
								 else if(Room_Temperature_zhengfu[0]==0x80)
											 UART3TxBuf[10+i*8]=0x2d;//-;
									UART3TxBuf[11+i*8]=0x30+Room_Temperature[0]/10;
									UART3TxBuf[12+i*8]=0x30+Room_Temperature[0]%10;
							}
							else if(i%4==1)//CO2数值显示
							{
								 /********文本长度 *****/
									UART3TxBuf[7+i*8]=0x00;
									UART3TxBuf[8+i*8]=0x04;
								 /********文本内容*****/
									UART3TxBuf[9+i*8]=0x30+Room_CO2[0]/1000;
									UART3TxBuf[10+i*8]=0x30+(Room_CO2[0]/100)%10;
									UART3TxBuf[11+i*8]=0x30+(Room_CO2[0]/10)%10;//
									UART3TxBuf[12+i*8]=0x30+Room_CO2[0]%10;//
							}
							else if(i%4==2)//PM25数值显示
							{
								 /********文本长度 *****/
									UART3TxBuf[7+i*8]=0x00;
									UART3TxBuf[8+i*8]=0x04;
								 /********文本内容*****/
									UART3TxBuf[9+i*8]=0x20;
									UART3TxBuf[10+i*8]=0x30+Room_PM25[0]/100;
									UART3TxBuf[11+i*8]=0x30+(Room_PM25[0]/10)%10;//Temp_ge;
									UART3TxBuf[12+i*8]=0x30+Room_PM25[0]%10;//Temp_ge;
							}
							else if(i%4==3)//VOC数值显示
							{
								 /********文本长度 *****/
									UART3TxBuf[7+i*8]=0x00;
									UART3TxBuf[8+i*8]=0x04;
								 /********文本内容*****/
									UART3TxBuf[9+i*8]=0x20;
									UART3TxBuf[10+i*8]=0x20;//Temp_ge;
									UART3TxBuf[11+i*8]=0x30+Room_VOC[0]/10;//Temp_ge;
									UART3TxBuf[12+i*8]=0x30+Room_VOC[0]%10;//Temp_ge;
							}	
						}
						UART3TxBuf[37]=0xff;
						UART3TxBuf[38]=0xfc;
						UART3TxBuf[39]=0xff;
						UART3TxBuf[40]=0xff;
						g_tUart3.Txflag    =0x00;//串口3正在发送数据，请勿更新数据
						comSendBuf(COM3,UART3TxBuf,41);
						g_tUart3.Txflag    =0x01;//串口3发送数据完成
					//}
			}
			else if(bsp_CheckTimer(TIMER_Screen1_Text_Update))	  
			{     	
					Text1_Flag=0;
					bsp_StopTimer(TIMER_Screen1_Text_Update);	
			}
		}
//*******************开机延时30S计时,之后才开启空调，开启屏幕更新除传感器数值之外的更新***************//	
	if(TIMER_OPEN_DELAY_Flag==1)
	{	
			/********************屏幕首页更新工作状态******************************/
			if(State0_Flag==0)//7S中更新一次
			{
					State0_Flag=1;
				 UART3TxBuf[0]=0xee;
				 UART3TxBuf[1]=0xb1;
				 UART3TxBuf[2]=0x10;
				 UART3TxBuf[3]=0x00;
				 UART3TxBuf[4]=0x00;
				 UART3TxBuf[5]=0x00;
				 UART3TxBuf[6]=State0_Count+71;
				if(Room_Work_State[State0_Count]!=0)
				{
					UART3TxBuf[7]=0xb9;
					UART3TxBuf[8]=0xa4;//工作
					UART3TxBuf[9]=0xd7;
					UART3TxBuf[10]=0xf7;
				}
				else
				{
					UART3TxBuf[7]=0xcd;
					UART3TxBuf[8]=0xa3;//停止
					UART3TxBuf[9]=0xd6;
					UART3TxBuf[10]=0xb9;
				}
				 UART3TxBuf[11]=0xff;
				 UART3TxBuf[12]=0xfc;
				 UART3TxBuf[13]=0xff;
				 UART3TxBuf[14]=0xff;
				 g_tUart3.Txflag    =0x00;//串口3正在发送数据，请勿更新数据
				 comSendBuf(COM3,UART3TxBuf,15);
				 State0_Count++;
				 if(State0_Count==5)
					 State0_Count=0;						
			}

			/****************屏幕第二页按键状态指示更新*****************************/
			if(State1_Flag==0)//5s更新一次
			{
					State1_Flag=1;
				  bsp_StartTimer(TIMER_Screen1_State_Update,500);
				   //自动情况下才更新第二页按键状态
		      if((Set_ConditionMode==0)||(Set_ConditionMode==1)||(Set_ConditionMode==2))
           {
							if(State1_Count==0)//制冷状态
							{
								if(Current_ConditionMode_Sta ==1)
								{
									SET_Button_State(0x01,0x04,0x01);
								}
								else 
								{
									SET_Button_State(0x01,0x04,0x00);
								}
							}
							else if(State1_Count==1)//制热状态
							{
									if(Current_ConditionMode_Sta==2)
									{
										SET_Button_State(0x01,0x05,0x01);
									}
									else
									{
										SET_Button_State(0x01,0x05,0x00);
									}
							}
							else if(State1_Count==2)//净化状态
							{//&&((Current_ConditionMode_Sta ==1)||(Current_ConditionMode_Sta ==2))
									if(JingHua_Force==1)//净化状态
									{
										SET_Button_State(0x01,0x06,0x01);
									}
									else 
									{					
										SET_Button_State(0x01,0x06,0x00);
									}
							}
							else if(State1_Count==3)//新风状态
							{
								 if(XinFeng_Force==1)//新风状态
								{
									 SET_Button_State(0x01,0x07,0x01);
								}
								else 
								{
									 SET_Button_State(0x01,0x07,0x00);
								}
							}
							else if(State1_Count==4)//强制加热
							{
								 if(Heat_Sta)//强制加热
								 {
									 SET_Button_State(0x01,0x08,0x01);
								 }
								 else 
								 {
									 SET_Button_State(0x01,0x08,0x00);
								 }
							}
							else if(State1_Count==5)//风速状态
							{
								if(Set_Condition_OpenSwitch==0)//如果空调外机不工作，那么此时风速显示为零
								{
									SET_JinDU(0);//根据风速设定进度条
								}
								else if(Set_WindSpeed==1)
								{
									SET_JinDU(33);//根据风速设定进度条
								}
								else if(Set_WindSpeed==2)
								{
									SET_JinDU(66);
								}
								else if(Set_WindSpeed==3)
								{
									SET_JinDU(99);
								}
							}		
							State1_Count++;
				  }
					else
					{
						State1_Count=0;
					}
			}
			else if(bsp_CheckTimer(TIMER_Screen1_State_Update))	  
			{   
						State1_Flag=0;
						bsp_StopTimer(TIMER_Screen1_State_Update); 
				    if(State1_Count==6)
							 State1_Count=0;
			}
			
			/******************屏幕首页舒适度,只和PM25有关***********************/
			if(Feel_Flag==0)//19S更新一次
			{
					Feel_Flag=1;
					bsp_StartTimer(TIMER_Screen0_Feel_Update,1900);
					UART3TxBuf[0]=0xee;
					UART3TxBuf[1]=0xb1;
					UART3TxBuf[2]=0x10;
					UART3TxBuf[3]=0x00;
					UART3TxBuf[4]=0x00;
					UART3TxBuf[5]=0x00;
					UART3TxBuf[6]=Feel_Count+26;
					if(Room_CO2[Feel_Count]<Set_CO2-100)
					{
							UART3TxBuf[7]=0xD3;//优
							UART3TxBuf[8]=0XC5;
					}
					else if(Room_CO2[Feel_Count]>Set_CO2+100)
					{
							UART3TxBuf[7]=0xC1;//良
							UART3TxBuf[8]=0XBC;
					}
					UART3TxBuf[9]=0xff;
					UART3TxBuf[10]=0xfc;
					UART3TxBuf[11]=0xff;
					UART3TxBuf[12]=0xff;
					g_tUart3.Txflag    =0x00;//串口3正在发送数据，请勿更新数据
					comSendBuf(COM3,UART3TxBuf,13);
					Feel_Count++;	
				}
			else if(bsp_CheckTimer(TIMER_Screen0_Feel_Update))	  
			{   
						Feel_Flag=0;
						bsp_StopTimer(TIMER_Screen0_Feel_Update);
				    if(Feel_Count==5)
							Feel_Count=0;
			}
			/*******************首页和第二页故障代码*******************************/
			if(Fault_Flag==0)//3s更新一次
			{
					 Fault_Flag=1;
					 bsp_StartTimer(TIMER_Screen_Fault_Update,300);
					 //if(g_tUart3.Txflag==0x01)//是否串口3正在空闲状态
					 //{
						 UART3TxBuf[0]=0xee;
						 UART3TxBuf[1]=0xb1;
				     UART3TxBuf[2]=0x10;
				    //屏幕画面地址
						 UART3TxBuf[3]=0x00;
				     UART3TxBuf[4]=Fault_Addr_Count;
						//控件地址
						 UART3TxBuf[5]=0x00;
						 UART3TxBuf[6]=0x1F;
						 if(Fault_Mode>10)//10以下代码留作内机报警
						 {		
						    UART3TxBuf[7]=0x4a;//10以上的显示报错代码
						    UART3TxBuf[8]=0x45;							 
								UART3TxBuf[9]=Fault_Mode/10+0x30;
								UART3TxBuf[10]=Fault_Mode%10+0x30;				
						 }
             else	
						 {
								UART3TxBuf[7]=0x20;//显示空格
						    UART3TxBuf[8]=0x20;			
							  UART3TxBuf[9]=0x20;
								UART3TxBuf[10]=0x20;	
						 }       							 
						 UART3TxBuf[11]=0xff;
						 UART3TxBuf[12]=0xfc;
						 UART3TxBuf[13]=0xff;
						 UART3TxBuf[14]=0xff;
						 g_tUart3.Txflag    =0x00;//串口3正在发送数据，请勿更新数据
						 comSendBuf(COM3,UART3TxBuf,15);
					   Fault_Addr_Count++;
			}
			else if(bsp_CheckTimer(TIMER_Screen_Fault_Update))
			{
					 Fault_Flag=0;
					 bsp_StopTimer(TIMER_Screen0_State_Update);
				   if(Fault_Addr_Count==2)
							 Fault_Addr_Count=0;
			}
			/*******************空调的读写操作*******************************/
			if(RT_Condition_Flag==0)//4S更新一次
				{
						 RT_Condition_Flag=1;
						 bsp_StartTimer(TIMER_RT_Condition, 400);//4S
						 Write_Air_Condition(); 
						 WORK_Count=0;			
						 for (i=0;i<4;i++)//开机命令后30S后检查分控器是否在工作
						 {
								if(Room_Wind_Feedback[i])
								{
										WORK_Count++;
								}	
								if(WORK_Count!=0)
								{
									 Set_Condition_OpenSwitch_sta=1;//开空调外机
								}
								else
								{
									 Set_Condition_OpenSwitch_sta=0;//开空调外机
								}
						 }
					}
					else if(bsp_CheckTimer(TIMER_RT_Condition))	   
					{
							RT_Condition_Flag=0;
							bsp_StopTimer(TIMER_RT_Condition);
					}	 		
		}
	else
	{
		  //RT_Condition_Flag=0;
			Set_Condition_OpenSwitch_sta=0;
			Set_Condition_OpenSwitch=0;
		  if(RT_Condition_Flag==0)//4S更新一次
			{
						 RT_Condition_Flag=1;
						 bsp_StartTimer(TIMER_RT_Condition, 400);//4S
						 Write_Air_Condition(); //为防止一次数据没有写进空调，需要多次关闭空调
			}
			else if(bsp_CheckTimer(TIMER_RT_Condition))	   
			{
							RT_Condition_Flag=0;
							bsp_StopTimer(TIMER_RT_Condition);
			}	
	}
	
		
}
/*
*********************************************************************************************************
*	函 数 名: Uart_ReciveProcess
*	功能说明: 接收到新的数据
*	形    参: _byte 接收到的新数据
*	返 回 值: 无
*********************************************************************************************************
*/
void Uart_ReciveProcess(void)
{
	uint16_t ret = 0;
	uint8_t Device_ID ;
	//触摸屏通讯接收处理，只有屏幕点击后触发才能够执行下部分程序
	if(g_tUart3.Rxflag==1)
	{
			g_tUart3.Rxflag=0;	
			//系统函数，将后一个数组中的数据拷贝一定的长度到前一个数组中
			memcpy(UART3RxBuf,g_tUart3.pRxBuf,g_tUart3.RxCount);
		  UART3RxCount=g_tUart3.RxCount;
			g_tUart3.RxCount=0;
		  //命令解析
			ret = Comm_Explain();
			if(ret==0x02)//文本控件的话，先接收设定的数值
			{
				//设定值，只是客厅的值
				if(g_Pdu.RegHold[0]!=0)
				{  Set_Temperature = g_Pdu.RegHold[0];}
				else 
				{Set_Temperature=22;}
				if(g_Pdu.RegHold[1]!=0)
				{   Set_PM25        = g_Pdu.RegHold[1];}
				else
				{Set_PM25=50;}
				if(g_Pdu.RegHold[2]!=0)
				{  Set_Humidity    = g_Pdu.RegHold[2];}
				if(g_Pdu.RegHold[3])
				{  Set_CO2         = g_Pdu.RegHold[3];}
				else
				{Set_CO2=1000;}
				if(g_Pdu.RegHold[4]!=0)
				{  Set_VOC         = g_Pdu.RegHold[4];}
				else
				{Set_VOC=25;}
			}
			else if(ret == 0x01)//按钮控件的话，首先要判断
			{
				if(!(g_Pdu.BitCoil[0]&0x10))
				{
					Set_OpenSwitch=0;//处于关机状态
				}
				else if((g_Pdu.BitCoil[0]&0x10)||(Set_OpenSwitch==1))//开机关机
				{
					 Set_OpenSwitch=1;
					//如果是开机的话，那么就要判断是自动模式还是手动模式，开机默认自动
          if(g_Pdu.BitCoil[0]&0x20)
					{
						Set_ConditionMode=0;//自动模式||(Set_ConditionMode ==0)，默认通风模式
						g_Pdu.BitCoil[0]&=~0x2000;//清除制热状态位
						g_Pdu.BitCoil[0]&=~0x1000;//清楚制冷状态位
						g_Pdu.BitCoil[0]&=~0x80;//清除超强状态位
						g_Pdu.BitCoil[0]&=~0x400;//清楚净化状态位
						g_Pdu.BitCoil[0]&=~0x100;//清除新风状态位
				  }
				  //******************手动情况下各种按钮情况***************//
					else  if((!(g_Pdu.BitCoil[0]&0x20))||(Set_ConditionMode ==3)||(Set_ConditionMode ==4)||(Set_ConditionMode ==5))
					{			
						Set_ConditionMode = 5;//手动模式下的通风
						JingHua_Force = 1;//手动情况下可以把尽管关掉
						if(g_Pdu.BitCoil[0]&0x1000)
						{
							Set_ConditionMode=3;//手动情况下的制冷
							g_Pdu.BitCoil[0]&=~0x2000;//清除制热状态位
							g_Pdu.BitCoil[0]|=0x1000;
							
							g_Pdu.BitCoil[0]&=~0x400;//手动情况下制热，冷肯定是要风机肯定要工作，即默认是净化模 式
							g_Pdu.BitCoil[0]|=0x400;
							JingHua_Force = 1;			
							g_Pdu.BitCoil[0]&=~0x80;//制冷情况下是不可能超强制热的	
						}
						else if(g_Pdu.BitCoil[0]&0x2000)//手动情况下的制热
						{
							Set_ConditionMode=4;
							g_Pdu.BitCoil[0]&=~0x1000;
							g_Pdu.BitCoil[0]|=0x2000;
							
							g_Pdu.BitCoil[0]&=~0x400;//手动情况下制热，冷肯定是要风机肯定要工作，即默认是净化模式
							g_Pdu.BitCoil[0]|=0x400;      
              JingHua_Force = 1;
						}
						//手动情况下可以调整风速，这个和制冷制热无关，只和循环风机有关，制冷制热模式，循环风机肯定会转起来
						if(g_Pdu.BitCoil[0]&0x01)
						{
								g_Pdu.BitCoil[0]&=~0x06;
								//低速
								Set_WindSpeed = 1;
						}
	          else if(g_Pdu.BitCoil[0]&0x02)
						{
								g_Pdu.BitCoil[0]&=~0x05;
								//中速
								Set_WindSpeed = 2;
						}
						else if(g_Pdu.BitCoil[0]&0x04)
						{ 
								g_Pdu.BitCoil[0]&=~0x03;
								//高速
								Set_WindSpeed = 3;
						}
						//如果手动情况下，分控器风阀全部关掉，则默认打开客厅的风阀
						if(Set_WindSpeed != 0)
						{//这个需要开启客厅风阀
								if((Room_Wind_Feedback[0]==0)&&(Room_Wind_Feedback[1]==0)&&(Room_Wind_Feedback[2]==0)&&(Room_Wind_Feedback[3]==0))
									  Room_Wind_Command[0]=1;
						}
						//超强加热,必须是在制热模式下才能启动
						if(Set_ConditionMode==4)
						{
							  if(g_Pdu.BitCoil[0]&0x80)
								{
									Set_HeatSwitch_sta=1;  
								}
								else
								{
									g_Pdu.BitCoil[0]&=~0x80;
									Set_HeatSwitch = 0;
									Set_HeatSwitch_sta=0;
									Set_HeatSwitch_Count=0;
								}
						}
						else 
						{
							g_Pdu.BitCoil[0]&=~0x80;
							Set_HeatSwitch = 0;
							Set_HeatSwitch_sta=0;
							Set_HeatSwitch_Count=0;
						}
							//手动新风新风
						if(g_Pdu.BitCoil[0]&0x100)
						{
							  XinFeng_Force = 1;
						}
						else
						{
							  XinFeng_Force= 0;
						}	
						//即没有新风，有没有制冷制热，可以手动点掉净化
						JingHua_Force = 1;
						if((Set_ConditionMode!=3)&&(Set_ConditionMode!=4)&&(XinFeng_Force==0))
						{
									//	手动净化					
								if(g_Pdu.BitCoil[0]&0x400)
								{
									 JingHua_Force = 1;
								}
								else
								{
									 JingHua_Force = 0;
								}			
						}
				 }		
			}
		}		
	}			
//****************************485发送处理部分********************/
	//485接收处理部分
  if(g_tUart5.Rxflag==1)
	{
		g_tUart5.Rxflag=0;
		memcpy(UART5RxBuf,g_tUart5.pRxBuf,g_tUart5.RxCount);
		UART5RxCount=g_tUart5.RxCount;
		g_tUart5.RxCount=0;
			//确认帧头
		if(UART5RxBuf[0] == 0xff)
		{
				//确认校验和
				  if(UART5RxBuf[1]==0x10)//针对性传感器
					{
						if(UART5RxBuf[2]==0x01)//对应其地址是0x01
						{
							if(UART5RxBuf[3]==0x01)//检查是否存在
								 Room_Work_State[4] = 0x01; //模块正在工作
							else if(UART5RxBuf[3]==0x02)
							{
								 Room_Temperature_zhengfu[4]=UART5RxBuf[4];
								 Room_Temperature[4]   = UART5RxBuf[5];
								 Room_Humidity[4]      = ((uint16_t)UART5RxBuf[6]<<8)|UART5RxBuf[7];
								 Room_CO2[4]           = ((uint16_t)UART5RxBuf[8]<<8)|UART5RxBuf[9];
								 Room_VOC[4] = ((uint16_t)UART5RxBuf[11]<<10)|UART5RxBuf[11];
								 Room_PM25[4]= ((uint16_t)UART5RxBuf[12]<<8)|UART5RxBuf[13];
								 Room_Work_State[4]    = 0x01; //模块正在工作
							}
						}	
					}		
					else if(UART5RxBuf[1]==0x20)//说明是分控器返回的数据
					{
						 Device_ID=UART5RxBuf[2];
			       if(UART5RxBuf[3]==0x02)
						 {
							 Room_Temperature_zhengfu[Device_ID-1]   =UART5RxBuf[4];
							 Room_Temperature[Device_ID-1]   = UART5RxBuf[5];
							 Room_Humidity[Device_ID-1]      = ((uint16_t)UART5RxBuf[6]<<8)|UART5RxBuf[7];
							 Room_CO2[Device_ID-1]           = ((uint16_t)UART5RxBuf[8]<<8)|UART5RxBuf[9];
							 Room_VOC[Device_ID-1]           = ((uint16_t)UART5RxBuf[10]<<8)|UART5RxBuf[11];
							 Room_PM25[Device_ID-1]          = ((uint16_t)UART5RxBuf[12]<<8)|UART5RxBuf[13];
							 Room_Work_State[Device_ID-1]    = UART5RxBuf[14]&0x01;//分控器工作状态
							 Room_Wind_Feedback[Device_ID-1] = UART5RxBuf[14]&0x02;//风阀开关状态
						 }
          } 
      }		
		}
}

/*
*********************************************************************************************************
*	函 数 名: Control_Task
*	功能说明: 实时数据刷新
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Control_Task(void)
{
	uint16_t buf[44];
	//系统开关机
	static uint8_t  Set_OpenSwitch_Bak = 0;
	//主板控制
	static uint16_t Room_Cnt = 0;
	       uint8_t  i =0;
//检查分控器在线个数，决定主机开关与否

	//首先必须开机
	if(Set_OpenSwitch)
	{   
		    if((Set_ConditionMode==0)||(Set_ConditionMode==1)||(Set_ConditionMode==2))
				{
						//自动模式,下的新排风机的运行，自动情况下的新排风机，新风机运行条件肯定是看CO2和VOC的值
						//自动模式情况下判断温度,是根据室外的温度来判断的,是否制冷制热
						//自动模式情况下温度参考户外的温度值，在16-28度之间不进行制冷制热
					if(Room_Temperature_zhengfu[4]==0x00)//正温度系数
					{
						if((Room_Temperature[4]>16)&&(Room_Temperature[4]<26))
						{
							 Set_ConditionMode=0;
						}
						//室外温度传感器的值
						else if((Room_Temperature[4]>26)||(Room_Temperature[4]==26))
						{
							Set_ConditionMode=1;//自动情况下的制冷
							g_Pdu.BitCoil[0]&=~0x80;//制冷情况下是不可能超强制热的
							JingHua_Force = 1;//制冷必定代表着净化
						}
						else if((Room_Temperature[4]<16)||(Room_Temperature[4]==16))
						{
									Set_ConditionMode=2;//自动情况下的制热
									JingHua_Force = 1;//制热必定代表着净化	
						}
					}
					else 	if(Room_Temperature_zhengfu[4]==0x80)//负温度，那么一定是制热
					{
						      Set_ConditionMode=2;//自动情况下的制热
									JingHua_Force = 1;//制热必定代表着净化	
					}
					else//防止其他情况下的错误，设置为通风模式
					{
						      Set_ConditionMode=0;//
									JingHua_Force = 1;//循环风机转，那么就属于净化模式
					}
					//自动模式情况下判断CO2,PM25,VOC,开启或关闭新排风机
					for(i=0;i<4;i++)
					{
							if((Room_CO2[i]>(Set_CO2+100))||(Room_VOC[i]>(Set_VOC+3)))
					    {  
								 CO2_VOC_Over_cnt++;
							}
							else if((Room_CO2[i]<(Set_CO2-100))&&(Room_VOC[i]<(Set_VOC-3)))
							{
								if(CO2_VOC_Over_cnt>0)
									CO2_VOC_Over_cnt--;
							}
							if(Room_PM25[i]>(Set_PM25+10))
							{
								 PM25_Over_cnt++;
							}
							else if(Room_PM25[i]<(Set_PM25-10))
							{
								if(PM25_Over_cnt>0)
									PM25_Over_cnt--;
							}
					}
					if(CO2_VOC_Over_cnt!=0)
					{
						 CO2_VOC_Over_flag=1;
					}
					else 
					{
						 CO2_VOC_Over_flag=0;
					}
					if(PM25_Over_cnt!=0)
					{
						 PM25_Over_flag=1;
					}
					else
					{
						 PM25_Over_flag=0;
					}
					if((CO2_VOC_Over_flag==1)||(PM25_Over_flag==1))
					{   //自动情况下即不制冷也不制热，即为通风模式
							if(Set_ConditionMode==0)
							{
									if(Set_Condition_OpenSwitch_sta==1)
									{
										Set_Condition_OpenSwitch=1;
										JingHua_Force=1;
									}
									else 
									{
										Set_Condition_OpenSwitch=0;
										JingHua_Force=0;
									}
							 }
							if(CO2_VOC_Over_flag==1)
								XinFeng_Force=1;
					}	
					else if(CO2_VOC_Over_flag==0)
					{
							XinFeng_Force=0;
							if(PM25_Over_flag==0)
							{//自动情况下即不制冷也不制热，即为通风模式
								   if(Set_ConditionMode==0)
							     {
										  Set_Condition_OpenSwitch=0;///////////////////////////////////////////////////////////////关机命令，空调开关
							     }
							 }		
					}
						//读取在线的从机个数
						Room_Cnt = 0;				
						for(i=0;i<4;i++)
						{
							if(Room_Wind_Feedback[i])
							{
									Room_Cnt++;
							}
						}				
						if(Room_Cnt==4)
						{
								Set_WindSpeed=3;//4个风阀全部打开高速
						}
						else if(Room_Cnt==3)
						{
							  Set_WindSpeed=2;//3个风阀打开开启中速模式
						}
						else if((Room_Cnt==2)||(Room_Cnt==1))
						{
							  Set_WindSpeed=1;//2个风阀以下打开，包括两个设置低速
						}	
            else	
						{
							  Set_Condition_OpenSwitch=0;//开空调外机
						}							
						/***********************自动模式下的制冷********************************/
						if(Set_ConditionMode ==1)
						{
							 //关闭新风电加热，关闭空调电加热
								AIR_ELECTRIC_HEAT_OFF;
								ELECTRIC_HEAT1_OFF;
								ELECTRIC_HEAT2_OFF;
								ELECTRIC_HEAT3_OFF;
								Heat_Sta    = 0;//超强制热
								Set_HeatSwitch=0;
								//这个地方*****留待更新SPI写空调改为通风模式***********/
							  if( Set_Condition_OpenSwitch_sta==1)
								{
									Set_Condition_OpenSwitch=1;
								}
								else 
								{
									Set_Condition_OpenSwitch=0;
								}
						}		   
						/*************************自动模式下的制热********************************/
					  if(Set_ConditionMode==2)
						{
							  if( Set_Condition_OpenSwitch_sta==1)//根据风阀的状态来开机外籍状态
								{
									Set_Condition_OpenSwitch=1;
								}
								else 
								{
									Set_Condition_OpenSwitch=0;
								}
						}
						else
						{
							Set_HeatSwitch=0;
						}
				}
			  else if((Set_ConditionMode==3)||(Set_ConditionMode ==4)||(Set_ConditionMode ==5))
				{ 
					  //手动情况下的制冷不需要特别处理，只需要设定温度和开空调外机
	           if(Set_ConditionMode ==3)
						 {
							 Set_HeatSwitch = 0;
							  if( Set_Condition_OpenSwitch_sta==1)//根据风阀的状态来开机外机状态
								{
									Set_Condition_OpenSwitch=1;
								}
								else 
								{
									Set_Condition_OpenSwitch=0;
								}
						 }
						/*************************手动模式下的制热**********************************/				
						 else if(Set_ConditionMode ==4)
						 {
								if( Set_Condition_OpenSwitch_sta==1)
								{
									Set_Condition_OpenSwitch=1;
								}
								else 
								{
									Set_Condition_OpenSwitch=0;
								}
								//从SPI里面读出来的反馈值
								if(Current_ConditionMode_Sta == 2)
								{
									//超强电加热,2MIN延迟
									if((Set_HeatSwitch_Count==120000)&&(Set_HeatSwitch_sta==1))
									{
										 Set_HeatSwitch = 1;
									}
									else if(Set_HeatSwitch_sta==0)
									{
										Set_HeatSwitch = 0;
									}
									if(Set_HeatSwitch)
									{	
										//循环风机必须打开，只要打开了制热状态，循环风机肯定处于工作状态，只是需要设定风速大小
										if(Set_WindSpeed != 0)
										{	
												ELECTRIC_HEAT1_ON;
												ELECTRIC_HEAT2_ON;
												ELECTRIC_HEAT3_ON;
												Heat_Sta = 1;
										}
									}
									else
									{
												ELECTRIC_HEAT1_OFF;
												ELECTRIC_HEAT2_OFF;
												ELECTRIC_HEAT3_OFF;
												Heat_Sta = 0;
									}
								}
								else
								{
									      ELECTRIC_HEAT1_OFF;
												ELECTRIC_HEAT2_OFF;
												ELECTRIC_HEAT3_OFF;
												Set_HeatSwitch = 0;
								}
							 
							}	
						 else if(Set_ConditionMode ==5)
						 {	
							 Set_HeatSwitch = 0;
							 if(Set_Condition_OpenSwitch_sta==1)//根据风阀的状态来开机外籍状态
							 {
										 if((Set_ConditionMode!=3)&&(Set_ConditionMode!=4)&&(JingHua_Force==0)&&(XinFeng_Force==0))
										 {
											  Set_Condition_OpenSwitch=0;//关掉空调外机
										 }
										 else
										 {
											  Set_Condition_OpenSwitch=1;
										 }
							}
							 else 
							 {
									Set_Condition_OpenSwitch=0;
							 }	
									
						 }	 
			  }
				if(Set_ConditionMode==2)
				{
					for(i=0;i<4;i++)
					{//这个地方应该判断完再去置位，或者清除
						if((Room_CO2[i]>(Set_CO2+100))||(Room_VOC[i]>(Set_VOC+3)))
						{
							XinFeng_Force=1;
							break;
						}
						else if((Room_CO2[i]<(Set_CO2-100))&&(Room_VOC[i]<(Set_VOC-3)))//12月15号添加，自动模式制热下新风机不能关闭
						{
							XinFeng_Force=0;
						}
					}		
				}
			//只要任意一个风阀打开就要更新新风状态,且无报错信息
        if(((Room_Wind_Feedback[0]!=0)||(Room_Wind_Feedback[1]!=0)||(Room_Wind_Feedback[2]!=0)||(Room_Wind_Feedback[3]!=0))&&(Fault_Mode<=10))				
			  {
					if(XinFeng_Force)
					{	
								//排风机开
								EXHAUST_AIR_ON;
								//新风机开
								FRESH_AIR_ON;	
								//排风阀开
								EXHAUST_AIR_POS_ON;
								EXHAUST_AIR_NEG_OFF;
								//新风阀开
								FRESH_AIR_POS_ON;
								FRESH_AIR_NEG_OFF;
								XinFeng_Sta = 1;
					}	
					else
					{
								//排风机关
								EXHAUST_AIR_OFF;
								//新风机开
								FRESH_AIR_OFF;	
								//排风阀关
								EXHAUST_AIR_POS_OFF;
								EXHAUST_AIR_NEG_ON;
								//新风阀关
								FRESH_AIR_POS_OFF;
								FRESH_AIR_NEG_ON;
								XinFeng_Sta = 0;
					}
					if(Set_HeatSwitch==1)
					{
						ELECTRIC_HEAT1_ON;
						ELECTRIC_HEAT2_ON;
						ELECTRIC_HEAT3_ON;
						Heat_Sta = 1;
					}
					else
					{
						 ELECTRIC_HEAT1_OFF;
						 ELECTRIC_HEAT2_OFF;
						 ELECTRIC_HEAT3_OFF;
						 Heat_Sta = 0;
					}
					
			  }
			  else//有报错，或者无分控器开启，关闭新排风机，和超强制热，并清除状态
			  {
			          //排风机关
								EXHAUST_AIR_OFF;
								//新风机关
								FRESH_AIR_OFF;	
								//排风阀关
								EXHAUST_AIR_POS_OFF;
								EXHAUST_AIR_NEG_ON;
								//新风阀关
								FRESH_AIR_POS_OFF;
								FRESH_AIR_NEG_ON;
								XinFeng_Sta = 0;
				 
								ELECTRIC_HEAT1_OFF;
								ELECTRIC_HEAT2_OFF;
								ELECTRIC_HEAT3_OFF;
								//超强制热相关标志位，包括手动和自动
								Heat_Sta = 0;
								Pre_Heat_Flag=0;
								Pre_Heat_Count=0;
								Heat_Wait_Count=0;
								Heat_Wait_Flag=0;
								//手动，强制加热
								Set_HeatSwitch_Count=0;
								Set_HeatSwitch_sta = 0;
								
			 }
	}
	else
	{
		HIGH_CYCLE_OFF;
		MID_CYCLE_OFF;
		FRESH_AIR_OFF;
		HUMIDIFIER_OFF;
		EXHAUST_AIR_OFF;
		AIR_ELECTRIC_HEAT_OFF;
		ELECTRIC_HEAT1_OFF;
		ELECTRIC_HEAT2_OFF;
		ELECTRIC_HEAT3_OFF;
	  EXHAUST_AIR_POS_OFF;
		EXHAUST_AIR_NEG_OFF;
		FRESH_AIR_POS_OFF;
		FRESH_AIR_NEG_OFF;
	  //超强制热相关标志位
		Heat_Sta = 0;
		Pre_Heat_Flag=0;
		Pre_Heat_Count=0;
		Heat_Wait_Count=0;
		Heat_Wait_Flag=0;
		Set_HeatSwitch_Count=0;
    Set_HeatSwitch_sta = 0;
		//净化和新风相关变量
		XinFeng_Sta = 0;
		JingHua_Sta = 0;
		JingHua_Force=0;
		XinFeng_Force=0;
		
		TIMER_OPEN_DELAY_Flag=0;//开机延时30S标志位清除
		TIMER_OPEN_DELAY_Count=0;//开机延时30S计数
		Set_WindSpeed = 1;	    //再次开机保持最低速	
		Room_Wind_Command[0]=0; //客厅风阀关闭命令，0状态不给出命令
		Set_ConditionMode=0;
		Current_ConditionMode_Sta = 0;	
		Set_Condition_OpenSwitch=0;//空调外机启动开关
		Set_Condition_OpenSwitch_sta=0;//空调外机启动状态
		g_Pdu.BitCoil[0]|=0x20;//再次开机设为自动模式	
  }
	//只在状态变化时执行，
	if(Set_OpenSwitch != Set_OpenSwitch_Bak)
	{
		Set_OpenSwitch_Bak = Set_OpenSwitch;
	
		if(Set_OpenSwitch == 0)
		{
				g_Pdu.RegHold[0] = Set_Temperature;
				g_Pdu.RegHold[1] = Set_PM25;
			  g_Pdu.RegHold[2] = Set_Humidity;
				g_Pdu.RegHold[3] = Set_CO2;
				g_Pdu.RegHold[4] = Set_VOC;		
				//房间环境参数，四个房间，外加一个室外环境参数，存储下来，准备关机预存数据
				for(i=0;i<5;i++)
				{
					g_Pdu.RegHold[7+5*i] = Room_Temperature[i];
					g_Pdu.RegHold[8+5*i] = Room_Humidity[i];
					g_Pdu.RegHold[9+5*i] = Room_PM25[i]>>8;//高位在前
          g_Pdu.RegHold[10+5*i] = Room_PM25[i]<<8;//地位在后	 					
					g_Pdu.RegHold[11+5*i] = Room_CO2[i]>>8;
					g_Pdu.RegHold[12+5*i] = Room_CO2[i]<<8;
					g_Pdu.RegHold[13+5*i] = Room_VOC[i];
				}	
				//g_Pdu.RegHold[42]= g_Pdu.BitCoil[0];//各种模式（包括自动，制冷，制冷，净化，新风，超强制热，除湿，风速）
				//g_Pdu.RegHold[43] = g_Pdu.BitCoil[1];//这个暂时还没用到
				memcpy(buf,g_Pdu.RegHold,44);        //拷贝数组
				bsp_WriteCpuFlash(PARA_SAVE_ADDR, (uint8_t*)buf, 44);//存入到FLASH
    }
		//每次开机，都进入自动模式
		else if(Set_OpenSwitch == 1)
		{	
				g_Pdu.BitCoil[0]|=0x20;
//			g_Pdu.BitCoil[0] &=~0x07;
//			g_Pdu.BitCoil[0] |= 0x01;
//			Set_WindSpeed = 1;	
    }
  }	  
	//过滤清洁
	g_Pdu.BitInp[0] &=~0x1c0;
	if(Current_FilterClean == 1)
	{
		g_Pdu.BitInp[0] |= 0x40;
	}
	else if(Current_FilterClean == 2)
	{
		g_Pdu.BitInp[0] |= 0x80;
	}
	else if(Current_FilterClean == 3)
	{
		g_Pdu.BitInp[0] |= 0x100;
	}	
 }

//计算spi累加和
uint8_t SPI_CheckSum(uint8_t *buf,uint8_t cnt)
{
	uint8_t  i = 0;
	uint16_t sum = 0;
	for(i=0;i<cnt;i++)
	{
			sum += buf[i];
  }
	return sum;
}

//计算分控器累加和
uint8_t Room_CheckSum(uint8_t *buf,uint8_t cnt)
{
	uint8_t  i = 0;
	uint8_t sum = 0;
	for(i=0;i<cnt;i++)
	{
			sum += buf[i];
  }
	return (sum^0xa8);
}
/*
*********************************************************************************************************
*	函 数 名: Room_Board_Write
*	功能说明: 分控器和传感器操作
*	形    参: addr，模块地址
*	返 回 值: 无
*********************************************************************************************************
*/
void Room_Board_Write(uint8_t target,uint8_t addr,uint8_t cmd)
{
			//轮询4个从机
		UART5TxBuf[0]  = 0xff;
		UART5TxBuf[1]  = target;//0x01对传感器模块操作，0x02对分控器操作
		UART5TxBuf[2]  = addr;  //风控器地址
		UART5TxBuf[3]  = cmd;   //命令，0x01传感器状态，0X02读取传感器监测值，0x03开分阀，0x04关分阀
		UART5TxBuf[4]  = 0x00;
		UART5TxBuf[5]  = 0x00;
		UART5TxBuf[6]  = 0x00;
		UART5TxBuf[7]  = 0x00;
		UART5TxBuf[8]  = 0x00;
		UART5TxBuf[9]  = 0x00;
		UART5TxBuf[10] = 0x00;
		UART5TxBuf[11] = 0x00;
		UART5TxBuf[12] = 0x00;
		UART5TxBuf[13] = 0x00;
		UART5TxBuf[14] = Room_CheckSum(UART5TxBuf,15);//校验和
		UART5TxBuf[15] = 0x88;
	  g_tUart5.Txflag     =0x00;
		comSendBuf(COM5,UART5TxBuf,16);
}
/*
*********************************************************************************************************
*	函 数 名: Write_Air_Condition
*	功能说明: 控制空调
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Write_Air_Condition(void)
{
	   uint8_t  i = 0;
		 uint8_t  temp = 0;
	   uint8_t  temp1 = 0;
		 uint8_t  wbuf[26];
     uint8_t  rbuf[26];	
	
			wbuf[0]  = 0xa5;
			wbuf[1]  = 0x48;
			wbuf[2]  = 0x0e;
			wbuf[3]  = 0;
			wbuf[4]  = 0x77;  //室温对应的AD值
	
			if(Set_WindSpeed==1)
			{
				  temp &=~0x6a;
				  temp |= 0x60;//低风
			}
		  else if(Set_WindSpeed==2)
			{
				  temp &=~0x6a;//中风
				  temp |= 0x40;
			}
		  else if(Set_WindSpeed==3)
			{
				  temp &=~0x6a;//高风
				  temp |= 0x20;
			}
			//Set_Condition_OpenSwitch=0;
			if(Set_Condition_OpenSwitch==1)//开机
			{
				 temp=temp+0x02;
			}
			else              
			{
				 temp =temp+0x01;
      }
			wbuf[5]  =temp; 
			//模式设置
			if((Set_ConditionMode == 1)||(Set_ConditionMode == 3))
			{
				//制冷模式
				temp &=~0xe0;
				temp |= 0x20;
      }
			else if((Set_ConditionMode == 2)||(Set_ConditionMode == 4))
			{
				//制热模式
				temp &=~0xe0;
				temp |= 0x80;
      }	
		  else if(((Set_ConditionMode ==5)&&((XinFeng_Force==1)||(JingHua_Force==1)))||(Set_ConditionMode ==0))
			{//即不制冷也不制热，且风速不为零，那么改为通风模式,或者是自动清华下的通风就是通风模式
				//通风模式
				temp &=~0xe0;
				temp |= 0x60;
			}
      if((Set_Temperature >= 16)&&(Set_Temperature <= 32))
			{
				//设定温度
				temp &=~0x1f;
				temp |= (Set_Temperature-16);
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
			SPI1_RW(208,wbuf,rbuf);
			//判断接收到的帧头是否正确,且开机状态
			if((rbuf[0] == 0xa5)&&(Set_Condition_OpenSwitch))
			{
				temp = (rbuf[7]>>5)&0x07;
				if(temp == 0)
				{
					//自动模式
					Current_ConditionMode_Sta = 0;
        }
				else if(temp == 1)
				{
					//制冷模式
					Current_ConditionMode_Sta = 1;
        }
				else if(temp == 3)
				{
					//通风模式
					Current_ConditionMode_Sta = 3;
        }
				else if(temp == 4)
				{
					//制热模式
					Current_ConditionMode_Sta = 2;
        }
				//防冷风、冷冻功能
        Condition_M10_Sta	= (rbuf[10]>>3)&0x03;	
			}
			//T1反馈温度值
			Temperature_T1=rbuf[5];
			
			if((rbuf[6]&0x20)&&(rbuf[6]&0x40))
			{
				Current_WindSpeed=1;//低速
			}	
		  else if(rbuf[6]&0x40)
			{
				 Current_WindSpeed=2;//中速
			}
		  else if(rbuf[6]&0x20)
			{
				Current_WindSpeed=3;//高速
			}			
			//空调报错类型
			temp1=0x01;
			for(i=0;i<8;i++)
			{
				if(rbuf[20]&temp1)
				{
					Fault_Mode=11+i;//空调错误类型，只记录第一个错误类型
					break;
				}
			  temp1=temp1<<1;			
			}
}
