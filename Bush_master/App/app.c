#include "bsp_global.h"
#include "app.h"
#include "spi.h"
#include "tempature.h"
#include "ads1232.h"
#include "comm.h"
//�����յ���ʱ30S��־λ
uint8_t	TIMER_OPEN_DELAY_Flag=0;
//�����յ���ʱ30S����
uint16_t	TIMER_OPEN_DELAY_Count=0;
//����״̬��0���رգ�1���򿪣�
uint8_t  JingHua_Sta = 0;
//ǿ�ƾ�����0���رգ�1���򿪣�
uint8_t  JingHua_Force = 0;
//�·�״̬��0���رգ�1���򿪣�
uint8_t  XinFeng_Sta = 0;
//ǿ���·磬0���رգ�1���򿪣�
uint8_t  XinFeng_Force = 0;
//��ǰ�յ�ģʽ״̬,0���Զ���1�����䣻2������;
uint8_t  Current_ConditionMode_Sta = 0;
//��ǰ�������,1:�죻2���ƣ�3���̣�
uint8_t  Current_FilterClean = 0;
//��ǰ����,1�����٣�2������;3:���٣�0:�ر�
uint8_t  Current_WindSpeed = 0;
//���õ��¶�û�и���
uint16_t Set_Temperature = 22;
//����ʪ��
uint16_t Set_Humidity =50;
//���ö�����̼Ũ��
uint16_t Set_CO2 = 1000;
//����PM2.5Ũ��
uint16_t Set_PM25 =25;
//����VOCŨ��
uint16_t Set_VOC = 25;
//���÷���,1�����٣�2������;3:���٣�0:�ر�
uint8_t  Set_WindSpeed = 1;
//���ù���ģʽ��0:�յ�ģʽ��1���Ӽ�ģʽ;2:���ģʽ��3����������ģʽ��
uint8_t  Set_WorkMode = 0;
//�����������أ�0���رգ�1����,�����ڻ������
uint8_t  Set_OpenSwitch = 0;
//�յ������������ 0���رգ�1����
uint8_t  Set_Condition_OpenSwitch = 0;
//�յ������������״̬ 0���رգ�1����
uint8_t  Set_Condition_OpenSwitch_sta=0;
//��ǿ����״̬
uint8_t  Heat_Sta = 0;
//�Զ�ģʽ�µ�Ԥ���ȹ���
uint8_t  Pre_Heat_Flag = 0;
uint8_t  Pre_Heat_Count = 0;
//�Զ�ģʽ�³�ǿԤ���ȵȴ���־λ
uint8_t  Heat_Wait_Flag = 0;
uint32_t Heat_Wait_Count = 0;
//�ֶ�ģʽ�µĳ�ǿ���ȱ�־λ�ͼ���
uint32_t Set_HeatSwitch_Count=0;
uint8_t  Set_HeatSwitch_sta = 0;
//��ǿ����,0���رգ�1���򿪣�
uint8_t  Set_HeatSwitch = 0;//��ǿ���ȵ��ܿ���
//�յ�ģʽ�л���0���Զ���1������,2:����,3:�ֶ�ģʽ�µ����䣬4���ֶ�ģʽ�µ�����,5:�ֶ�������޲���
uint8_t  Set_ConditionMode = 0;
//����������¶ȣ���5������5��������ʾ��������
int16_t Room_Temperature[5];//�¶��и���
uint8_t Room_Temperature_zhengfu[5];//�¶��и���
//���������ʪ�ȣ���5��,��5��������ʾ��������
uint16_t Room_Humidity[5];
//���������PM2.5����5��,��5��������ʾ��������
uint16_t Room_PM25[5];
//���������CO2����5��,��5��������ʾ��������
uint16_t Room_CO2[5];
//���������VOC����5��,��5��������ʾ��������
uint16_t Room_VOC[5];
//���������ͷ������״̬����,0���رգ�1���򿪣�
uint8_t  Room_Wind_Feedback[5];
//��������Ĺ���״̬
uint8_t  Room_Work_State[5];
//�緧�����ر�����,��Ҫ�ǿ����ķ緧����
uint8_t  Room_Wind_Command[5];
//����硢��˪����
uint8_t  Condition_M10_Sta = 0; 
//������
uint8_t  Fault_Mode=0;

uint8_t  CO2_VOC_Over_cnt=0;//�������꼼��
uint8_t  CO2_VOC_Over_flag=0;//���������־λ
uint8_t  PM25_Over_cnt=0;//�������꼼��
uint8_t  PM25_Over_flag=0;//���������־λ

uint8_t  State0_Flag = 0;//�ֿ�������״̬
uint8_t  State0_Count = 0;//����״̬��ؼ���
uint16_t State0_TIMER_Count = 0;//����״̬��ؼ���



int8_t  Temperature_T1=0;
/*
*********************************************************************************************************
*	�� �� ��: Init_Hard
*	����˵��: ��ʼ��Ӳ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Init_Hard(void)
{
	//��ʼ������
	bsp_InitDin();
	//��ʼ�����
	bsp_InitDout();
	//��ʼ����ʱ��
	bsp_InitTimer();
	//��ʼ������
	bsp_InitUart();
	//��ʼ��SPI
	SPI1_Init();
}
/*
*********************************************************************************************************
*	�� �� ��: Init_Soft
*	����˵��: ��ʼ�����
*	��    ��: ��
*	�� �� ֵ: ��
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
	//��ȡflash�е�����
	bsp_ReadCpuFlash(PARA_SAVE_ADDR, (uint8_t*)buf, 44);
	//�ж��Ƿ��ǳ����ϵ�
//	if((buf[0] != 0xffff)&&(buf[0] != 0))
//	{
//		//5���价������
//		
//		//g_Pdu.BitCoil[0] = buf[42];
//		g_Pdu.BitCoil[1] = buf[43];
//  }
		for(i=0;i<44;i++)
		{
			g_Pdu.RegHold[i]  =0;// buf[i];
    }
	  TIMER_OPEN_DELAY_Flag=0;//������ʱ30S��־λ���
		TIMER_OPEN_DELAY_Count=0;//������ʱ30S����
		Set_Temperature=22;//Ĭ�Ͽ����г�ʼֵ
		Set_PM25=50;
		Set_CO2=1000;
		Set_VOC=25;
		Set_WindSpeed = 1;	    //�ٴο������������
		JingHua_Force=0;
		XinFeng_Force=0;
		Room_Wind_Command[0]=0;//�����緧�ر����0״̬����������
		Set_ConditionMode=0;//�յ�ģʽ
		Set_Condition_OpenSwitch=0;//�յ������������
		Set_Condition_OpenSwitch_sta=0;//�յ��������״̬
		g_Pdu.BitCoil[0]|=0x20;//��ʼ����Ĭ�����Զ�ģʽ
	  g_Pdu.BitCoil[0] &=~0x07;//�ͷ�״̬λ
	  g_Pdu.BitCoil[0] |= 0x01;
	
	//Ĭ�Ϲ������,��ɫ
	Current_FilterClean = 3;
	//û�й��Ϸ�����ʼ��
	Fault_Mode=0;
	//����3���ڿ���״̬
	g_tUart3.Txflag=0x01;
}
/*
*********************************************************************************************************
*	�� �� ��: SET_Button_State
*	����˵��: ����������״̬���ã�������״̬����
*	��    ��: _
*	�� �� ֵ: ��
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
	   g_tUart3.Txflag    =0x00;//����3���ڷ������ݣ������������
		 comSendBuf(COM3,UART3TxBuf,12);
		 g_tUart3.Txflag    =0x01;//����3�����������
}
/*
*********************************************************************************************************
*	�� �� ��: SET_JinDU
*	����˵��: ���ý�������ֵ
*	��    ��: _
*	�� �� ֵ: ��
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
		 UART3TxBuf[6]=0x11;   //��������
		 UART3TxBuf[7]=0x00;  
	   UART3TxBuf[8]=0x00;  
	   UART3TxBuf[9]=0x00;  
	   UART3TxBuf[10]=DAT;  
		 UART3TxBuf[11]=0xff;
		 UART3TxBuf[12]=0xfc;
		 UART3TxBuf[13]=0xff;
		 UART3TxBuf[14]=0xff;
	   g_tUart3.Txflag    =0x00;//����3���ڷ������ݣ������������
		 comSendBuf(COM3,UART3TxBuf,15);
	   g_tUart3.Txflag    =0x01;//����3�����������
}
/*
*********************************************************************************************************
*	�� �� ��: Uart_SendProcess
*	����˵��: �������ݴ�����Ҫ�������������ʾ��485�������·�
*	��    ��: _
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Uart_SendProcess(void)
{
	//��Ҫ�ϴ���ҳ���������ݣ�����״̬�����ʶȣ����ϴ��룬�ռ�״̬
	uint8_t  i=0;
	static uint8_t  Text0_Flag = 0; //��һҳ������ֵ����
	static uint8_t  Text1_Flag = 0; //�ڶ�ҳ���������ֵ����
	  
	static uint8_t  State1_Flag = 0;//�Զ�ģʽ����µĿ��ư�����״̬��ʾ
	static uint8_t  Feel_Flag = 0;  //���ʶ�
	static uint8_t  Fault_Flag = 0; //��������ϴ���־λ
	static uint8_t  T_485_Cmd_ONOFF_Flag = 0;//����485���ػ���־λ
	static uint8_t  T_485_Read_Flag = 0;  //�����������ݲ���
	static uint8_t  RT_Condition_Flag =1;//�Կյ�SPI��д������־λ
	static uint8_t  Fault_Addr_Count = 0; //�����������
  static uint8_t  Feel_Count = 0;       //���ʶ���ؼ���
	
	static uint8_t  State1_Count = 0;      //����״̬��ؼ���
  static uint8_t  Room_Read_addr=0;     //�����ַ�������ݵ�ַ
	static uint8_t  Room_Cmd_addr=0;     //�����ַ�������ݵ�ַ
	static uint8_t  Room_Cmd_addr_Count=0;//��θ�������·�����
	       uint8_t  target_temp=0;        //���������ֿ���Ŀ�����ѡ��
	static uint8_t  Set_OpenSwitch_temp=0;
	static uint8_t  Set_OpenSwitch_flag=0;//��ȡ���ݺͷ��������л���־λ,ֻ�����л�����״̬
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
	/*************���ֿ�����ӻ��⴫��������**************/
		if(Set_OpenSwitch_flag==1)
		{
				if(T_485_Read_Flag ==0)
				{
						T_485_Read_Flag =1;
						bsp_StartTimer(TIMER_Read_485, 200);//2S ��ѯ�Ǹ�����ķֿ��������һ�����⴫��������ͼ��Ҳ����趨��2S
						Room_Read_addr++;
						
					  if(Room_Read_addr<5)
						{
							target_temp=0x02;//���������Ƿֿ���
							Room_Board_Write(target_temp,Room_Read_addr,0x02);	
						}	
            else if(Room_Read_addr==5)
						{
							target_temp=0x01;//���������Ǵ�����ģ��
							Room_Board_Write(target_temp,0x01,0x02);		
						}	
						else if(Room_Read_addr==5)
						{
							if((Room_Wind_Command[0]==1)&&((Set_ConditionMode ==0)||(Set_ConditionMode ==1)||(Set_ConditionMode ==2)
								||(Set_ConditionMode ==3)||(Set_ConditionMode ==4)||(Set_ConditionMode ==5))&&(Set_Condition_OpenSwitch==0))
							{
								Room_Board_Write(0x02,0x01,0x03);	//�������緧
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
	/*************���ֿ����������������´��������ֻ����״̬�ı��ʱ���ִ��*************/
		if(Set_OpenSwitch_temp!=Set_OpenSwitch)
		{
			 Set_OpenSwitch_flag=0;//���ڷ��͹ػ�ָ�� 
			 if(T_485_Cmd_ONOFF_Flag ==0)
			 {
					T_485_Cmd_ONOFF_Flag=1;
					bsp_StartTimer(TIMER_Cmd_485,200);//״̬�ı��ʱ���2S���͸���ͬ�ķֿ��������ź�
					Room_Cmd_addr++;                  //��ַ�ı�		
					if(Set_OpenSwitch==0)
					{
							Room_Board_Write(0x02,Room_Cmd_addr,0x05);//���Ƕ�����Room_Cmd_addr
							g_Pdu.BitCoil[0]&=~0x20;
							g_Pdu.BitCoil[0]|=0x20;//�ػ�֮����λ���ܹ�Ϊ��һ�ο��������Զ�ģʽ����־	
					}

			 }
			 else if(bsp_CheckTimer(TIMER_Cmd_485))	   
			 {
						T_485_Cmd_ONOFF_Flag =0;
						bsp_StopTimer(TIMER_Cmd_485);
				   //���û�н���״̬�л��ǻ�������������Ȼ��ֱ�ӷ��Ͷ����ݷ�������
						if(Room_Cmd_addr==4)              
						{
								Room_Cmd_addr=0;
								Room_Cmd_addr_Count++;
								if(Room_Cmd_addr_Count==2)       //�·����ػ���������Σ���ֹ���ݶ�ʧ
								{
									Room_Cmd_addr_Count=0;
									Set_OpenSwitch_temp=Set_OpenSwitch;
									
									if(Set_OpenSwitch==1)
									{
										while((g_tUart3.Txflag==0)||(g_tUart5.Txflag==0));//�ȴ�������ɺ󣬷�����������
										UART3TxBuf[0]=0xee;
										UART3TxBuf[1]=0xb1;
										UART3TxBuf[2]=0x10;
										UART3TxBuf[3]=0x00;
										UART3TxBuf[4]=0x00;
										UART3TxBuf[5]=0x00;
										UART3TxBuf[6]=0x20;
										UART3TxBuf[7]=0xb9;//�ѿ�������ʾ���ء�
										UART3TxBuf[8]=0xd8;
										UART3TxBuf[9]=0xff;
										UART3TxBuf[10]=0xfc;
										UART3TxBuf[11]=0xff;
										UART3TxBuf[12]=0xff;
										g_tUart3.Txflag     =0x00;//����3���ڷ������ݣ������������,�ֿ����͹鷢�ͣ�����黺�棬���ڻ�����Ҫ�ֿ���
										comSendBuf(COM3,UART3TxBuf,13);
										while((g_tUart3.Txflag==0)||(g_tUart5.Txflag==0));//�ȴ�������ɺ󣬷�����������
										UART3TxBuf[0]=0xee;
										UART3TxBuf[1]=0xb1;
										UART3TxBuf[2]=0x10;
										UART3TxBuf[3]=0x00;
										UART3TxBuf[4]=0x01;
										UART3TxBuf[5]=0x00;
										UART3TxBuf[6]=0x02;
										UART3TxBuf[7]=0xb9;//�ѿ�������ʾ���ء�
										UART3TxBuf[8]=0xd8;
										UART3TxBuf[9]=0xff;
										UART3TxBuf[10]=0xfc;
										UART3TxBuf[11]=0xff;
										UART3TxBuf[12]=0xff;
										g_tUart3.Txflag     =0x00;//����3���ڷ������ݣ������������,�ֿ����͹鷢�ͣ�����黺�棬���ڻ�����Ҫ�ֿ���
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
											UART3TxBuf[7]=0xbf;//�ѹػ�����ʾ������
											UART3TxBuf[8]=0xaa;
											UART3TxBuf[9]=0xff;
											UART3TxBuf[10]=0xfc;
											UART3TxBuf[11]=0xff;
											UART3TxBuf[12]=0xff;
											g_tUart3.Txflag     =0x00;//����3���ڷ������ݣ������������,�ֿ����͹鷢�ͣ�����黺�棬���ڻ�����Ҫ�ֿ���
											comSendBuf(COM3,UART3TxBuf,13);
										  while((g_tUart3.Txflag==0)||(g_tUart5.Txflag==0));
											UART3TxBuf[0]=0xee;
											UART3TxBuf[1]=0xb1;
											UART3TxBuf[2]=0x10;
											UART3TxBuf[3]=0x00;
											UART3TxBuf[4]=0x01;
											UART3TxBuf[5]=0x00;
											UART3TxBuf[6]=0x02;
											UART3TxBuf[7]=0xbf;//�ѹػ�����ʾ������
											UART3TxBuf[8]=0xaa;
											UART3TxBuf[9]=0xff;
											UART3TxBuf[10]=0xfc;
											UART3TxBuf[11]=0xff;
											UART3TxBuf[12]=0xff;
											g_tUart3.Txflag     =0x00;//����3���ڷ������ݣ������������,�ֿ����͹鷢�ͣ�����黺�棬���ڻ�����Ҫ�ֿ���
											comSendBuf(COM3,UART3TxBuf,13);
									}
								} 
					  }  			
				}
		 } 
    else if(Set_OpenSwitch_temp==Set_OpenSwitch)
				       Set_OpenSwitch_flag=1;         //�´�������ɱ�־λ,���Է��Ͷ�ȡ������	
	  if(Set_OpenSwitch==1)
	  { 
			/********************��Ļ��ҳ����������ֵ����**********************/
			if(Text0_Flag==0)//17s����һ��
			{
					Text0_Flag=1;
					//if(g_tUart3.Txflag==0x01)//�Ƿ񴮿�3���ڿ���״̬
					//{	
						//17S������Ļһ��
						bsp_StartTimer(TIMER_Screen0_Text_Update,1700);
						UART3TxBuf[0]=0xee;
						UART3TxBuf[1]=0xb1;
						UART3TxBuf[2]=0x12;
						UART3TxBuf[3]=0x00;
						UART3TxBuf[4]=0x00;
						for(i=0;i<25;i++)
							{
								/********�ؼ���ַ ******/
								UART3TxBuf[5+i*8]=0x00;
								UART3TxBuf[6+i*8]=i+1;
								if(i%5==0)//�¶�ֵ��ʾ
								{
									 /********�ı����� *****/
										UART3TxBuf[7+i*8]=0x00;
										UART3TxBuf[8+i*8]=0x04;
									 /********�ı�����*****/
										UART3TxBuf[9+i*8]=0x20;
									  if(Room_Temperature_zhengfu[i/5]==0x00)
										   UART3TxBuf[10+i*8]=0x20;//Temp_ge;
										else if(Room_Temperature_zhengfu[i/5]==0x80)
											 UART3TxBuf[10+i*8]=0x2d;//Temp_ge;
										UART3TxBuf[11+i*8]=0x30+Room_Temperature[i/5]/10;//Temp_ge;
										UART3TxBuf[12+i*8]=0x30+Room_Temperature[i/5]%10;//Temp_ge;
								}
								else if(i%5==1)//ʪ��ֵ��ʾ
								{
									 /********�ı����� *****/
										UART3TxBuf[7+i*8]=0x00;
										UART3TxBuf[8+i*8]=0x04;
									 /********�ı�����*****/
										UART3TxBuf[9+i*8]=0x20;
										UART3TxBuf[10+i*8]=0x20;//Temp_ge;
										UART3TxBuf[11+i*8]=0x30+Room_Humidity[i/5]/10;//Temp_ge;
										UART3TxBuf[12+i*8]=0x30+Room_Humidity[i/5]%10;//Temp_ge;
								}
								else if(i%5==2)//PM2.5��ֵ��ʾ
								{
									 /********�ı����� *****/
										UART3TxBuf[7+i*8]=0x00;
										UART3TxBuf[8+i*8]=0x04;
									 /********�ı�����*****/
										UART3TxBuf[9+i*8]=0x20;
										UART3TxBuf[10+i*8]=0x30+Room_PM25[i/5]/100;
										UART3TxBuf[11+i*8]=0x30+(Room_PM25[i/5]/10)%10;//Temp_ge;
										UART3TxBuf[12+i*8]=0x30+Room_PM25[i/5]%10;//Temp_ge;
								}
								else if(i%5==3)//CO2��ֵ��ʾ
								{
									 /********�ı����� *****/
										UART3TxBuf[7+i*8]=0x00;
										UART3TxBuf[8+i*8]=0x04;
									 /********�ı�����*****/
										UART3TxBuf[9+i*8]=0x30+Room_CO2[i/5]/1000;
										UART3TxBuf[10+i*8]=0x30+(Room_CO2[i/5]/100)%10;
										UART3TxBuf[11+i*8]=0x30+(Room_CO2[i/5]/10)%10;//Temp_ge;
										UART3TxBuf[12+i*8]=0x30+Room_CO2[i/5]%10;//Temp_ge;
								}
								else if(i%5==4)
								{
									 /********�ı����� *****/
										UART3TxBuf[7+i*8]=0x00;
										UART3TxBuf[8+i*8]=0x04;
									 /********�ı�����*****/
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
							g_tUart3.Txflag     =0x00;//����3���ڷ������ݣ������������,�ֿ����͹鷢�ͣ�����黺�棬���ڻ�����Ҫ�ֿ���
							comSendBuf(COM3,UART3TxBuf,209);
						  g_tUart3.Txflag    =0x01;//����3�����������
				//	}
			}
			else if(bsp_CheckTimer(TIMER_Screen0_Text_Update))	  
			{     
						Text0_Flag=0;
						bsp_StopTimer(TIMER_Screen0_Text_Update);
			}
				/***************��Ļ�ڶ�ҳ�Ĵ��������ֵ����************************/
			if(Text1_Flag==0)//9s����һ��
			{
					Text1_Flag=1;
					bsp_StartTimer(TIMER_Screen1_Text_Update,900);
					//if(g_tUart3.Txflag==0x01)//�Ƿ񴮿�3���ڿ���״̬
					//{
						UART3TxBuf[0]=0xee;
						UART3TxBuf[1]=0xb1;
						UART3TxBuf[2]=0x12;
						UART3TxBuf[3]=0x00;
						UART3TxBuf[4]=0x01;
						for(i=0;i<4;i++)//4���������ļ��ֵ
						{
							/********�ؼ���ַ ******/
							UART3TxBuf[5+i*8]=0x00;
							UART3TxBuf[6+i*8]=i+24;			
							if(i%4==0)//�¶�ֵ��ʾ
							{
								 /********�ı����� *****/
									UART3TxBuf[7+i*8]=0x00;
									UART3TxBuf[8+i*8]=0x04;
								 /********�ı�����*****/
									UART3TxBuf[9+i*8]=0x20;
								 if(Room_Temperature_zhengfu[0]==0x00)
										   UART3TxBuf[10+i*8]=0x20;//Temp_ge;
								 else if(Room_Temperature_zhengfu[0]==0x80)
											 UART3TxBuf[10+i*8]=0x2d;//-;
									UART3TxBuf[11+i*8]=0x30+Room_Temperature[0]/10;
									UART3TxBuf[12+i*8]=0x30+Room_Temperature[0]%10;
							}
							else if(i%4==1)//CO2��ֵ��ʾ
							{
								 /********�ı����� *****/
									UART3TxBuf[7+i*8]=0x00;
									UART3TxBuf[8+i*8]=0x04;
								 /********�ı�����*****/
									UART3TxBuf[9+i*8]=0x30+Room_CO2[0]/1000;
									UART3TxBuf[10+i*8]=0x30+(Room_CO2[0]/100)%10;
									UART3TxBuf[11+i*8]=0x30+(Room_CO2[0]/10)%10;//
									UART3TxBuf[12+i*8]=0x30+Room_CO2[0]%10;//
							}
							else if(i%4==2)//PM25��ֵ��ʾ
							{
								 /********�ı����� *****/
									UART3TxBuf[7+i*8]=0x00;
									UART3TxBuf[8+i*8]=0x04;
								 /********�ı�����*****/
									UART3TxBuf[9+i*8]=0x20;
									UART3TxBuf[10+i*8]=0x30+Room_PM25[0]/100;
									UART3TxBuf[11+i*8]=0x30+(Room_PM25[0]/10)%10;//Temp_ge;
									UART3TxBuf[12+i*8]=0x30+Room_PM25[0]%10;//Temp_ge;
							}
							else if(i%4==3)//VOC��ֵ��ʾ
							{
								 /********�ı����� *****/
									UART3TxBuf[7+i*8]=0x00;
									UART3TxBuf[8+i*8]=0x04;
								 /********�ı�����*****/
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
						g_tUart3.Txflag    =0x00;//����3���ڷ������ݣ������������
						comSendBuf(COM3,UART3TxBuf,41);
						g_tUart3.Txflag    =0x01;//����3�����������
					//}
			}
			else if(bsp_CheckTimer(TIMER_Screen1_Text_Update))	  
			{     	
					Text1_Flag=0;
					bsp_StopTimer(TIMER_Screen1_Text_Update);	
			}
		}
//*******************������ʱ30S��ʱ,֮��ſ����յ���������Ļ���³���������ֵ֮��ĸ���***************//	
	if(TIMER_OPEN_DELAY_Flag==1)
	{	
			/********************��Ļ��ҳ���¹���״̬******************************/
			if(State0_Flag==0)//7S�и���һ��
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
					UART3TxBuf[8]=0xa4;//����
					UART3TxBuf[9]=0xd7;
					UART3TxBuf[10]=0xf7;
				}
				else
				{
					UART3TxBuf[7]=0xcd;
					UART3TxBuf[8]=0xa3;//ֹͣ
					UART3TxBuf[9]=0xd6;
					UART3TxBuf[10]=0xb9;
				}
				 UART3TxBuf[11]=0xff;
				 UART3TxBuf[12]=0xfc;
				 UART3TxBuf[13]=0xff;
				 UART3TxBuf[14]=0xff;
				 g_tUart3.Txflag    =0x00;//����3���ڷ������ݣ������������
				 comSendBuf(COM3,UART3TxBuf,15);
				 State0_Count++;
				 if(State0_Count==5)
					 State0_Count=0;						
			}

			/****************��Ļ�ڶ�ҳ����״ָ̬ʾ����*****************************/
			if(State1_Flag==0)//5s����һ��
			{
					State1_Flag=1;
				  bsp_StartTimer(TIMER_Screen1_State_Update,500);
				   //�Զ�����²Ÿ��µڶ�ҳ����״̬
		      if((Set_ConditionMode==0)||(Set_ConditionMode==1)||(Set_ConditionMode==2))
           {
							if(State1_Count==0)//����״̬
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
							else if(State1_Count==1)//����״̬
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
							else if(State1_Count==2)//����״̬
							{//&&((Current_ConditionMode_Sta ==1)||(Current_ConditionMode_Sta ==2))
									if(JingHua_Force==1)//����״̬
									{
										SET_Button_State(0x01,0x06,0x01);
									}
									else 
									{					
										SET_Button_State(0x01,0x06,0x00);
									}
							}
							else if(State1_Count==3)//�·�״̬
							{
								 if(XinFeng_Force==1)//�·�״̬
								{
									 SET_Button_State(0x01,0x07,0x01);
								}
								else 
								{
									 SET_Button_State(0x01,0x07,0x00);
								}
							}
							else if(State1_Count==4)//ǿ�Ƽ���
							{
								 if(Heat_Sta)//ǿ�Ƽ���
								 {
									 SET_Button_State(0x01,0x08,0x01);
								 }
								 else 
								 {
									 SET_Button_State(0x01,0x08,0x00);
								 }
							}
							else if(State1_Count==5)//����״̬
							{
								if(Set_Condition_OpenSwitch==0)//����յ��������������ô��ʱ������ʾΪ��
								{
									SET_JinDU(0);//���ݷ����趨������
								}
								else if(Set_WindSpeed==1)
								{
									SET_JinDU(33);//���ݷ����趨������
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
			
			/******************��Ļ��ҳ���ʶ�,ֻ��PM25�й�***********************/
			if(Feel_Flag==0)//19S����һ��
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
							UART3TxBuf[7]=0xD3;//��
							UART3TxBuf[8]=0XC5;
					}
					else if(Room_CO2[Feel_Count]>Set_CO2+100)
					{
							UART3TxBuf[7]=0xC1;//��
							UART3TxBuf[8]=0XBC;
					}
					UART3TxBuf[9]=0xff;
					UART3TxBuf[10]=0xfc;
					UART3TxBuf[11]=0xff;
					UART3TxBuf[12]=0xff;
					g_tUart3.Txflag    =0x00;//����3���ڷ������ݣ������������
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
			/*******************��ҳ�͵ڶ�ҳ���ϴ���*******************************/
			if(Fault_Flag==0)//3s����һ��
			{
					 Fault_Flag=1;
					 bsp_StartTimer(TIMER_Screen_Fault_Update,300);
					 //if(g_tUart3.Txflag==0x01)//�Ƿ񴮿�3���ڿ���״̬
					 //{
						 UART3TxBuf[0]=0xee;
						 UART3TxBuf[1]=0xb1;
				     UART3TxBuf[2]=0x10;
				    //��Ļ�����ַ
						 UART3TxBuf[3]=0x00;
				     UART3TxBuf[4]=Fault_Addr_Count;
						//�ؼ���ַ
						 UART3TxBuf[5]=0x00;
						 UART3TxBuf[6]=0x1F;
						 if(Fault_Mode>10)//10���´��������ڻ�����
						 {		
						    UART3TxBuf[7]=0x4a;//10���ϵ���ʾ�������
						    UART3TxBuf[8]=0x45;							 
								UART3TxBuf[9]=Fault_Mode/10+0x30;
								UART3TxBuf[10]=Fault_Mode%10+0x30;				
						 }
             else	
						 {
								UART3TxBuf[7]=0x20;//��ʾ�ո�
						    UART3TxBuf[8]=0x20;			
							  UART3TxBuf[9]=0x20;
								UART3TxBuf[10]=0x20;	
						 }       							 
						 UART3TxBuf[11]=0xff;
						 UART3TxBuf[12]=0xfc;
						 UART3TxBuf[13]=0xff;
						 UART3TxBuf[14]=0xff;
						 g_tUart3.Txflag    =0x00;//����3���ڷ������ݣ������������
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
			/*******************�յ��Ķ�д����*******************************/
			if(RT_Condition_Flag==0)//4S����һ��
				{
						 RT_Condition_Flag=1;
						 bsp_StartTimer(TIMER_RT_Condition, 400);//4S
						 Write_Air_Condition(); 
						 WORK_Count=0;			
						 for (i=0;i<4;i++)//���������30S����ֿ����Ƿ��ڹ���
						 {
								if(Room_Wind_Feedback[i])
								{
										WORK_Count++;
								}	
								if(WORK_Count!=0)
								{
									 Set_Condition_OpenSwitch_sta=1;//���յ����
								}
								else
								{
									 Set_Condition_OpenSwitch_sta=0;//���յ����
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
		  if(RT_Condition_Flag==0)//4S����һ��
			{
						 RT_Condition_Flag=1;
						 bsp_StartTimer(TIMER_RT_Condition, 400);//4S
						 Write_Air_Condition(); //Ϊ��ֹһ������û��д���յ�����Ҫ��ιرտյ�
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
*	�� �� ��: Uart_ReciveProcess
*	����˵��: ���յ��µ�����
*	��    ��: _byte ���յ���������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Uart_ReciveProcess(void)
{
	uint16_t ret = 0;
	uint8_t Device_ID ;
	//������ͨѶ���մ���ֻ����Ļ����󴥷����ܹ�ִ���²��ֳ���
	if(g_tUart3.Rxflag==1)
	{
			g_tUart3.Rxflag=0;	
			//ϵͳ����������һ�������е����ݿ���һ���ĳ��ȵ�ǰһ��������
			memcpy(UART3RxBuf,g_tUart3.pRxBuf,g_tUart3.RxCount);
		  UART3RxCount=g_tUart3.RxCount;
			g_tUart3.RxCount=0;
		  //�������
			ret = Comm_Explain();
			if(ret==0x02)//�ı��ؼ��Ļ����Ƚ����趨����ֵ
			{
				//�趨ֵ��ֻ�ǿ�����ֵ
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
			else if(ret == 0x01)//��ť�ؼ��Ļ�������Ҫ�ж�
			{
				if(!(g_Pdu.BitCoil[0]&0x10))
				{
					Set_OpenSwitch=0;//���ڹػ�״̬
				}
				else if((g_Pdu.BitCoil[0]&0x10)||(Set_OpenSwitch==1))//�����ػ�
				{
					 Set_OpenSwitch=1;
					//����ǿ����Ļ�����ô��Ҫ�ж����Զ�ģʽ�����ֶ�ģʽ������Ĭ���Զ�
          if(g_Pdu.BitCoil[0]&0x20)
					{
						Set_ConditionMode=0;//�Զ�ģʽ||(Set_ConditionMode ==0)��Ĭ��ͨ��ģʽ
						g_Pdu.BitCoil[0]&=~0x2000;//�������״̬λ
						g_Pdu.BitCoil[0]&=~0x1000;//�������״̬λ
						g_Pdu.BitCoil[0]&=~0x80;//�����ǿ״̬λ
						g_Pdu.BitCoil[0]&=~0x400;//�������״̬λ
						g_Pdu.BitCoil[0]&=~0x100;//����·�״̬λ
				  }
				  //******************�ֶ�����¸��ְ�ť���***************//
					else  if((!(g_Pdu.BitCoil[0]&0x20))||(Set_ConditionMode ==3)||(Set_ConditionMode ==4)||(Set_ConditionMode ==5))
					{			
						Set_ConditionMode = 5;//�ֶ�ģʽ�µ�ͨ��
						JingHua_Force = 1;//�ֶ�����¿��԰Ѿ��ܹص�
						if(g_Pdu.BitCoil[0]&0x1000)
						{
							Set_ConditionMode=3;//�ֶ�����µ�����
							g_Pdu.BitCoil[0]&=~0x2000;//�������״̬λ
							g_Pdu.BitCoil[0]|=0x1000;
							
							g_Pdu.BitCoil[0]&=~0x400;//�ֶ���������ȣ���϶���Ҫ����϶�Ҫ��������Ĭ���Ǿ���ģ ʽ
							g_Pdu.BitCoil[0]|=0x400;
							JingHua_Force = 1;			
							g_Pdu.BitCoil[0]&=~0x80;//����������ǲ����ܳ�ǿ���ȵ�	
						}
						else if(g_Pdu.BitCoil[0]&0x2000)//�ֶ�����µ�����
						{
							Set_ConditionMode=4;
							g_Pdu.BitCoil[0]&=~0x1000;
							g_Pdu.BitCoil[0]|=0x2000;
							
							g_Pdu.BitCoil[0]&=~0x400;//�ֶ���������ȣ���϶���Ҫ����϶�Ҫ��������Ĭ���Ǿ���ģʽ
							g_Pdu.BitCoil[0]|=0x400;      
              JingHua_Force = 1;
						}
						//�ֶ�����¿��Ե������٣���������������޹أ�ֻ��ѭ������йأ���������ģʽ��ѭ������϶���ת����
						if(g_Pdu.BitCoil[0]&0x01)
						{
								g_Pdu.BitCoil[0]&=~0x06;
								//����
								Set_WindSpeed = 1;
						}
	          else if(g_Pdu.BitCoil[0]&0x02)
						{
								g_Pdu.BitCoil[0]&=~0x05;
								//����
								Set_WindSpeed = 2;
						}
						else if(g_Pdu.BitCoil[0]&0x04)
						{ 
								g_Pdu.BitCoil[0]&=~0x03;
								//����
								Set_WindSpeed = 3;
						}
						//����ֶ�����£��ֿ����緧ȫ���ص�����Ĭ�ϴ򿪿����ķ緧
						if(Set_WindSpeed != 0)
						{//�����Ҫ���������緧
								if((Room_Wind_Feedback[0]==0)&&(Room_Wind_Feedback[1]==0)&&(Room_Wind_Feedback[2]==0)&&(Room_Wind_Feedback[3]==0))
									  Room_Wind_Command[0]=1;
						}
						//��ǿ����,������������ģʽ�²�������
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
							//�ֶ��·��·�
						if(g_Pdu.BitCoil[0]&0x100)
						{
							  XinFeng_Force = 1;
						}
						else
						{
							  XinFeng_Force= 0;
						}	
						//��û���·磬��û���������ȣ������ֶ��������
						JingHua_Force = 1;
						if((Set_ConditionMode!=3)&&(Set_ConditionMode!=4)&&(XinFeng_Force==0))
						{
									//	�ֶ�����					
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
//****************************485���ʹ�����********************/
	//485���մ�����
  if(g_tUart5.Rxflag==1)
	{
		g_tUart5.Rxflag=0;
		memcpy(UART5RxBuf,g_tUart5.pRxBuf,g_tUart5.RxCount);
		UART5RxCount=g_tUart5.RxCount;
		g_tUart5.RxCount=0;
			//ȷ��֡ͷ
		if(UART5RxBuf[0] == 0xff)
		{
				//ȷ��У���
				  if(UART5RxBuf[1]==0x10)//����Դ�����
					{
						if(UART5RxBuf[2]==0x01)//��Ӧ���ַ��0x01
						{
							if(UART5RxBuf[3]==0x01)//����Ƿ����
								 Room_Work_State[4] = 0x01; //ģ�����ڹ���
							else if(UART5RxBuf[3]==0x02)
							{
								 Room_Temperature_zhengfu[4]=UART5RxBuf[4];
								 Room_Temperature[4]   = UART5RxBuf[5];
								 Room_Humidity[4]      = ((uint16_t)UART5RxBuf[6]<<8)|UART5RxBuf[7];
								 Room_CO2[4]           = ((uint16_t)UART5RxBuf[8]<<8)|UART5RxBuf[9];
								 Room_VOC[4] = ((uint16_t)UART5RxBuf[11]<<10)|UART5RxBuf[11];
								 Room_PM25[4]= ((uint16_t)UART5RxBuf[12]<<8)|UART5RxBuf[13];
								 Room_Work_State[4]    = 0x01; //ģ�����ڹ���
							}
						}	
					}		
					else if(UART5RxBuf[1]==0x20)//˵���Ƿֿ������ص�����
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
							 Room_Work_State[Device_ID-1]    = UART5RxBuf[14]&0x01;//�ֿ�������״̬
							 Room_Wind_Feedback[Device_ID-1] = UART5RxBuf[14]&0x02;//�緧����״̬
						 }
          } 
      }		
		}
}

/*
*********************************************************************************************************
*	�� �� ��: Control_Task
*	����˵��: ʵʱ����ˢ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Control_Task(void)
{
	uint16_t buf[44];
	//ϵͳ���ػ�
	static uint8_t  Set_OpenSwitch_Bak = 0;
	//�������
	static uint16_t Room_Cnt = 0;
	       uint8_t  i =0;
//���ֿ������߸��������������������

	//���ȱ��뿪��
	if(Set_OpenSwitch)
	{   
		    if((Set_ConditionMode==0)||(Set_ConditionMode==1)||(Set_ConditionMode==2))
				{
						//�Զ�ģʽ,�µ����ŷ�������У��Զ�����µ����ŷ�����·�����������϶��ǿ�CO2��VOC��ֵ
						//�Զ�ģʽ������ж��¶�,�Ǹ���������¶����жϵ�,�Ƿ���������
						//�Զ�ģʽ������¶Ȳο�������¶�ֵ����16-28��֮�䲻������������
					if(Room_Temperature_zhengfu[4]==0x00)//���¶�ϵ��
					{
						if((Room_Temperature[4]>16)&&(Room_Temperature[4]<26))
						{
							 Set_ConditionMode=0;
						}
						//�����¶ȴ�������ֵ
						else if((Room_Temperature[4]>26)||(Room_Temperature[4]==26))
						{
							Set_ConditionMode=1;//�Զ�����µ�����
							g_Pdu.BitCoil[0]&=~0x80;//����������ǲ����ܳ�ǿ���ȵ�
							JingHua_Force = 1;//����ض������ž���
						}
						else if((Room_Temperature[4]<16)||(Room_Temperature[4]==16))
						{
									Set_ConditionMode=2;//�Զ�����µ�����
									JingHua_Force = 1;//���ȱض������ž���	
						}
					}
					else 	if(Room_Temperature_zhengfu[4]==0x80)//���¶ȣ���ôһ��������
					{
						      Set_ConditionMode=2;//�Զ�����µ�����
									JingHua_Force = 1;//���ȱض������ž���	
					}
					else//��ֹ��������µĴ�������Ϊͨ��ģʽ
					{
						      Set_ConditionMode=0;//
									JingHua_Force = 1;//ѭ�����ת����ô�����ھ���ģʽ
					}
					//�Զ�ģʽ������ж�CO2,PM25,VOC,������ر����ŷ��
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
					{   //�Զ�����¼�������Ҳ�����ȣ���Ϊͨ��ģʽ
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
							{//�Զ�����¼�������Ҳ�����ȣ���Ϊͨ��ģʽ
								   if(Set_ConditionMode==0)
							     {
										  Set_Condition_OpenSwitch=0;///////////////////////////////////////////////////////////////�ػ�����յ�����
							     }
							 }		
					}
						//��ȡ���ߵĴӻ�����
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
								Set_WindSpeed=3;//4���緧ȫ���򿪸���
						}
						else if(Room_Cnt==3)
						{
							  Set_WindSpeed=2;//3���緧�򿪿�������ģʽ
						}
						else if((Room_Cnt==2)||(Room_Cnt==1))
						{
							  Set_WindSpeed=1;//2���緧���´򿪣������������õ���
						}	
            else	
						{
							  Set_Condition_OpenSwitch=0;//���յ����
						}							
						/***********************�Զ�ģʽ�µ�����********************************/
						if(Set_ConditionMode ==1)
						{
							 //�ر��·����ȣ��رտյ������
								AIR_ELECTRIC_HEAT_OFF;
								ELECTRIC_HEAT1_OFF;
								ELECTRIC_HEAT2_OFF;
								ELECTRIC_HEAT3_OFF;
								Heat_Sta    = 0;//��ǿ����
								Set_HeatSwitch=0;
								//����ط�*****��������SPIд�յ���Ϊͨ��ģʽ***********/
							  if( Set_Condition_OpenSwitch_sta==1)
								{
									Set_Condition_OpenSwitch=1;
								}
								else 
								{
									Set_Condition_OpenSwitch=0;
								}
						}		   
						/*************************�Զ�ģʽ�µ�����********************************/
					  if(Set_ConditionMode==2)
						{
							  if( Set_Condition_OpenSwitch_sta==1)//���ݷ緧��״̬�������⼮״̬
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
					  //�ֶ�����µ����䲻��Ҫ�ر���ֻ��Ҫ�趨�¶ȺͿ��յ����
	           if(Set_ConditionMode ==3)
						 {
							 Set_HeatSwitch = 0;
							  if( Set_Condition_OpenSwitch_sta==1)//���ݷ緧��״̬���������״̬
								{
									Set_Condition_OpenSwitch=1;
								}
								else 
								{
									Set_Condition_OpenSwitch=0;
								}
						 }
						/*************************�ֶ�ģʽ�µ�����**********************************/				
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
								//��SPI����������ķ���ֵ
								if(Current_ConditionMode_Sta == 2)
								{
									//��ǿ�����,2MIN�ӳ�
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
										//ѭ���������򿪣�ֻҪ��������״̬��ѭ������϶����ڹ���״̬��ֻ����Ҫ�趨���ٴ�С
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
							 if(Set_Condition_OpenSwitch_sta==1)//���ݷ緧��״̬�������⼮״̬
							 {
										 if((Set_ConditionMode!=3)&&(Set_ConditionMode!=4)&&(JingHua_Force==0)&&(XinFeng_Force==0))
										 {
											  Set_Condition_OpenSwitch=0;//�ص��յ����
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
					{//����ط�Ӧ���ж�����ȥ��λ���������
						if((Room_CO2[i]>(Set_CO2+100))||(Room_VOC[i]>(Set_VOC+3)))
						{
							XinFeng_Force=1;
							break;
						}
						else if((Room_CO2[i]<(Set_CO2-100))&&(Room_VOC[i]<(Set_VOC-3)))//12��15����ӣ��Զ�ģʽ�������·�����ܹر�
						{
							XinFeng_Force=0;
						}
					}		
				}
			//ֻҪ����һ���緧�򿪾�Ҫ�����·�״̬,���ޱ�����Ϣ
        if(((Room_Wind_Feedback[0]!=0)||(Room_Wind_Feedback[1]!=0)||(Room_Wind_Feedback[2]!=0)||(Room_Wind_Feedback[3]!=0))&&(Fault_Mode<=10))				
			  {
					if(XinFeng_Force)
					{	
								//�ŷ����
								EXHAUST_AIR_ON;
								//�·����
								FRESH_AIR_ON;	
								//�ŷ緧��
								EXHAUST_AIR_POS_ON;
								EXHAUST_AIR_NEG_OFF;
								//�·緧��
								FRESH_AIR_POS_ON;
								FRESH_AIR_NEG_OFF;
								XinFeng_Sta = 1;
					}	
					else
					{
								//�ŷ����
								EXHAUST_AIR_OFF;
								//�·����
								FRESH_AIR_OFF;	
								//�ŷ緧��
								EXHAUST_AIR_POS_OFF;
								EXHAUST_AIR_NEG_ON;
								//�·緧��
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
			  else//�б��������޷ֿ����������ر����ŷ�����ͳ�ǿ���ȣ������״̬
			  {
			          //�ŷ����
								EXHAUST_AIR_OFF;
								//�·����
								FRESH_AIR_OFF;	
								//�ŷ緧��
								EXHAUST_AIR_POS_OFF;
								EXHAUST_AIR_NEG_ON;
								//�·緧��
								FRESH_AIR_POS_OFF;
								FRESH_AIR_NEG_ON;
								XinFeng_Sta = 0;
				 
								ELECTRIC_HEAT1_OFF;
								ELECTRIC_HEAT2_OFF;
								ELECTRIC_HEAT3_OFF;
								//��ǿ������ر�־λ�������ֶ����Զ�
								Heat_Sta = 0;
								Pre_Heat_Flag=0;
								Pre_Heat_Count=0;
								Heat_Wait_Count=0;
								Heat_Wait_Flag=0;
								//�ֶ���ǿ�Ƽ���
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
	  //��ǿ������ر�־λ
		Heat_Sta = 0;
		Pre_Heat_Flag=0;
		Pre_Heat_Count=0;
		Heat_Wait_Count=0;
		Heat_Wait_Flag=0;
		Set_HeatSwitch_Count=0;
    Set_HeatSwitch_sta = 0;
		//�������·���ر���
		XinFeng_Sta = 0;
		JingHua_Sta = 0;
		JingHua_Force=0;
		XinFeng_Force=0;
		
		TIMER_OPEN_DELAY_Flag=0;//������ʱ30S��־λ���
		TIMER_OPEN_DELAY_Count=0;//������ʱ30S����
		Set_WindSpeed = 1;	    //�ٴο������������	
		Room_Wind_Command[0]=0; //�����緧�ر����0״̬����������
		Set_ConditionMode=0;
		Current_ConditionMode_Sta = 0;	
		Set_Condition_OpenSwitch=0;//�յ������������
		Set_Condition_OpenSwitch_sta=0;//�յ��������״̬
		g_Pdu.BitCoil[0]|=0x20;//�ٴο�����Ϊ�Զ�ģʽ	
  }
	//ֻ��״̬�仯ʱִ�У�
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
				//���价���������ĸ����䣬���һ�����⻷���������洢������׼���ػ�Ԥ������
				for(i=0;i<5;i++)
				{
					g_Pdu.RegHold[7+5*i] = Room_Temperature[i];
					g_Pdu.RegHold[8+5*i] = Room_Humidity[i];
					g_Pdu.RegHold[9+5*i] = Room_PM25[i]>>8;//��λ��ǰ
          g_Pdu.RegHold[10+5*i] = Room_PM25[i]<<8;//��λ�ں�	 					
					g_Pdu.RegHold[11+5*i] = Room_CO2[i]>>8;
					g_Pdu.RegHold[12+5*i] = Room_CO2[i]<<8;
					g_Pdu.RegHold[13+5*i] = Room_VOC[i];
				}	
				//g_Pdu.RegHold[42]= g_Pdu.BitCoil[0];//����ģʽ�������Զ������䣬���䣬�������·磬��ǿ���ȣ���ʪ�����٣�
				//g_Pdu.RegHold[43] = g_Pdu.BitCoil[1];//�����ʱ��û�õ�
				memcpy(buf,g_Pdu.RegHold,44);        //��������
				bsp_WriteCpuFlash(PARA_SAVE_ADDR, (uint8_t*)buf, 44);//���뵽FLASH
    }
		//ÿ�ο������������Զ�ģʽ
		else if(Set_OpenSwitch == 1)
		{	
				g_Pdu.BitCoil[0]|=0x20;
//			g_Pdu.BitCoil[0] &=~0x07;
//			g_Pdu.BitCoil[0] |= 0x01;
//			Set_WindSpeed = 1;	
    }
  }	  
	//�������
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

//����spi�ۼӺ�
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

//����ֿ����ۼӺ�
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
*	�� �� ��: Room_Board_Write
*	����˵��: �ֿ����ʹ���������
*	��    ��: addr��ģ���ַ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Room_Board_Write(uint8_t target,uint8_t addr,uint8_t cmd)
{
			//��ѯ4���ӻ�
		UART5TxBuf[0]  = 0xff;
		UART5TxBuf[1]  = target;//0x01�Դ�����ģ�������0x02�Էֿ�������
		UART5TxBuf[2]  = addr;  //�������ַ
		UART5TxBuf[3]  = cmd;   //���0x01������״̬��0X02��ȡ���������ֵ��0x03���ַ���0x04�طַ�
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
		UART5TxBuf[14] = Room_CheckSum(UART5TxBuf,15);//У���
		UART5TxBuf[15] = 0x88;
	  g_tUart5.Txflag     =0x00;
		comSendBuf(COM5,UART5TxBuf,16);
}
/*
*********************************************************************************************************
*	�� �� ��: Write_Air_Condition
*	����˵��: ���ƿյ�
*	��    ��: ��
*	�� �� ֵ: ��
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
			wbuf[4]  = 0x77;  //���¶�Ӧ��ADֵ
	
			if(Set_WindSpeed==1)
			{
				  temp &=~0x6a;
				  temp |= 0x60;//�ͷ�
			}
		  else if(Set_WindSpeed==2)
			{
				  temp &=~0x6a;//�з�
				  temp |= 0x40;
			}
		  else if(Set_WindSpeed==3)
			{
				  temp &=~0x6a;//�߷�
				  temp |= 0x20;
			}
			//Set_Condition_OpenSwitch=0;
			if(Set_Condition_OpenSwitch==1)//����
			{
				 temp=temp+0x02;
			}
			else              
			{
				 temp =temp+0x01;
      }
			wbuf[5]  =temp; 
			//ģʽ����
			if((Set_ConditionMode == 1)||(Set_ConditionMode == 3))
			{
				//����ģʽ
				temp &=~0xe0;
				temp |= 0x20;
      }
			else if((Set_ConditionMode == 2)||(Set_ConditionMode == 4))
			{
				//����ģʽ
				temp &=~0xe0;
				temp |= 0x80;
      }	
		  else if(((Set_ConditionMode ==5)&&((XinFeng_Force==1)||(JingHua_Force==1)))||(Set_ConditionMode ==0))
			{//��������Ҳ�����ȣ��ҷ��ٲ�Ϊ�㣬��ô��Ϊͨ��ģʽ,�������Զ��廪�µ�ͨ�����ͨ��ģʽ
				//ͨ��ģʽ
				temp &=~0xe0;
				temp |= 0x60;
			}
      if((Set_Temperature >= 16)&&(Set_Temperature <= 32))
			{
				//�趨�¶�
				temp &=~0x1f;
				temp |= (Set_Temperature-16);
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
			SPI1_RW(208,wbuf,rbuf);
			//�жϽ��յ���֡ͷ�Ƿ���ȷ,�ҿ���״̬
			if((rbuf[0] == 0xa5)&&(Set_Condition_OpenSwitch))
			{
				temp = (rbuf[7]>>5)&0x07;
				if(temp == 0)
				{
					//�Զ�ģʽ
					Current_ConditionMode_Sta = 0;
        }
				else if(temp == 1)
				{
					//����ģʽ
					Current_ConditionMode_Sta = 1;
        }
				else if(temp == 3)
				{
					//ͨ��ģʽ
					Current_ConditionMode_Sta = 3;
        }
				else if(temp == 4)
				{
					//����ģʽ
					Current_ConditionMode_Sta = 2;
        }
				//����硢�䶳����
        Condition_M10_Sta	= (rbuf[10]>>3)&0x03;	
			}
			//T1�����¶�ֵ
			Temperature_T1=rbuf[5];
			
			if((rbuf[6]&0x20)&&(rbuf[6]&0x40))
			{
				Current_WindSpeed=1;//����
			}	
		  else if(rbuf[6]&0x40)
			{
				 Current_WindSpeed=2;//����
			}
		  else if(rbuf[6]&0x20)
			{
				Current_WindSpeed=3;//����
			}			
			//�յ���������
			temp1=0x01;
			for(i=0;i<8;i++)
			{
				if(rbuf[20]&temp1)
				{
					Fault_Mode=11+i;//�յ��������ͣ�ֻ��¼��һ����������
					break;
				}
			  temp1=temp1<<1;			
			}
}
