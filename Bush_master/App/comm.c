#include "bsp_global.h"
#include <string.h>
#include "Comm.h"
#include "crc.h"

uint8_t UART3RxBuf[RXBUFLEN];	         //接收缓冲区
uint8_t UART3RxCount=0;
uint8_t UART3TxBuf[TXBUFLEN];	           //发送缓冲区
uint8_t UART3RxBuf[TXBUFLEN];	           //发送缓冲区
uint8_t UART5RxCount=0;
uint8_t UART5TxBuf[TXBUFLEN];	           //发送缓冲区
uint8_t UART5RxBuf[TXBUFLEN];	           //发送缓冲区
struct _PDU	g_Pdu;
/**************************************************************
函数名：Comm_MbMtInit()
输入：
输出：
功能：  
***************************************************************/
void Comm_MbMtInit(void)
{	
	
}


/**************************************************************
函数名：Comm_MbSl_rep()
输入：
输出：成功则返回功能码，0代表失败
功能：对所接受到的协议命令进行解码，分为
***************************************************************/

uint16_t Comm_Explain(void)
{
	uint8_t  FuncCode;
	uint8_t  ErrCode=0;
//DecodeSentdat数组中的前MBTXBUFLEN个数都设置为零
	if(UART3RxCount<5)
	{
		return 0;
	}
	if((UART3RxBuf[0]!=0xee)||(UART3RxBuf[1]!=0xb1)||(UART3RxBuf[2]!=0x11)
		 ||(UART3RxBuf[UART3RxCount-4]!=0xff)||(UART3RxBuf[UART3RxCount-3]!=0xfc)
	   ||(UART3RxBuf[UART3RxCount-2]!=0xff)||(UART3RxBuf[UART3RxCount-1]!=0xff))
	{
		 ErrCode=0;
	}
	else 
	{
			 if(UART3RxBuf[7]==0x10)
				  FuncCode=0x01;//按钮控件
			 else if(UART3RxBuf[7]==0x11)
				  FuncCode=0x02;//文本控件
			 else if(UART3RxBuf[7]==0x12)
				  FuncCode=0x03;//进度条控件
			 else if(UART3RxBuf[7]==0x13)
				  FuncCode=0x04;//滑块控件
			 else if(UART3RxBuf[7]==0x14)
				  FuncCode=0x05;//仪表控件	 
	}
	if(FuncCode==0x01)//判断具体控件
	{
		 if((UART3RxBuf[3]==0x00)&&(UART3RxBuf[4]==0x00))
		 {
			 if((UART3RxBuf[5]==0x00)&&(UART3RxBuf[6]==0x54))//画面0的第84个控件，开关按钮
			 {
				 if(UART3RxBuf[9]==0x01)
					 g_Pdu.BitCoil[0]|=0x10;//开关按钮
				 else
					 g_Pdu.BitCoil[0]&=~0x10;
			 }
		 }
		 else if((UART3RxBuf[3]==0x00)&&(UART3RxBuf[4]==0x01))
		 {
			 if((UART3RxBuf[5]==0x00)&&(UART3RxBuf[6]==0x29))//画面0的第84个控件，开关按钮
			 {
				 if(UART3RxBuf[9]==0x01)
					 g_Pdu.BitCoil[0]|=0x10;
				 else
					 g_Pdu.BitCoil[0]&=~0x10;
			 }
			 else if((UART3RxBuf[5]==0x00)&&(UART3RxBuf[6]==0x03))//自动模式,手动模式
			 {
				 if(UART3RxBuf[9]==0x01)
				  g_Pdu.BitCoil[0]&=~0x20;
				 else 
					 g_Pdu.BitCoil[0]|=0x20;
			 }
			 else if ((UART3RxBuf[5]==0x00)&&(UART3RxBuf[6]==0x04))//判断手动情况的制冷
			 {
				 if(UART3RxBuf[9]==0x01)
				 {
					 g_Pdu.BitCoil[0]&=~0x2000;
					 g_Pdu.BitCoil[0]|=0x1000;
				 } 
				 else 
				 {
					 g_Pdu.BitCoil[0]&=~0x1000;
				 }
					
			 }
			 else if ((UART3RxBuf[5]==0x00)&&(UART3RxBuf[6]==0x05))//判断手动情况的制热
			 {
				  if(UART3RxBuf[9]==0x01)
					{
						 g_Pdu.BitCoil[0]|=0x2000;
						 g_Pdu.BitCoil[0]&=~0x1000;
					}
				  else 
					{
						 g_Pdu.BitCoil[0]&=~0x2000;
					}
			 }
			 else if((UART3RxBuf[5]==0x00)&&((UART3RxBuf[6]==0x0E)||(UART3RxBuf[6]==0x0F)|(UART3RxBuf[6]==0x10)))//风速调整
			 {
			    //待完善。。。。。。。。。。。。。。。。。
				 if(UART3RxBuf[6]==0x0E)
				 {
					  g_Pdu.BitCoil[0]&=~0x02;
					  g_Pdu.BitCoil[0]&=~0x04;
					  g_Pdu.BitCoil[0]|=0x01;//低档速度
				 }
				 else if(UART3RxBuf[6]==0x0f)
				 {
					  g_Pdu.BitCoil[0]&=~0x01;
					  g_Pdu.BitCoil[0]&=~0x04;
					  g_Pdu.BitCoil[0]|=0x02;//中档速度
				 }
				 else if(UART3RxBuf[6]==0x10)
				 {
					  g_Pdu.BitCoil[0]&=~0x01;
					  g_Pdu.BitCoil[0]&=~0x02;
					  g_Pdu.BitCoil[0]|=0x04;//高档速度
				 }
			 }
			 else if((UART3RxBuf[5]==0x00)&&(UART3RxBuf[6]==0x0D))//除湿模式
			 {
				  if(UART3RxBuf[9]==0x01)
				   g_Pdu.BitCoil[0]|=0x40;
				  else
					 g_Pdu.BitCoil[0]&=~0x40;
				 ;
			 }
			 else if((UART3RxBuf[5]==0x00)&&(UART3RxBuf[6]==0x08))//超强制热
			 {
				 if(UART3RxBuf[9]==0x01)
				   g_Pdu.BitCoil[0]|=0x80;
				 else
					 g_Pdu.BitCoil[0]&=~0x80;
			 }
			 else if((UART3RxBuf[5]==0x00)&&(UART3RxBuf[6]==0x07))//新风模式
			 {
				  if(UART3RxBuf[9]==0x01)
				   g_Pdu.BitCoil[0]|=0x100;
				  else
					 g_Pdu.BitCoil[0]&=~0x100;
			 }
			 else if((UART3RxBuf[5]==0x00)&&(UART3RxBuf[6]==0x06))//净化模式
			 {
				  if(UART3RxBuf[9]==0x01)
				   g_Pdu.BitCoil[0]|=0x400;
				  else
					 g_Pdu.BitCoil[0]&=~0x400;
			 } 
			 
		  }
	 }
	else if(FuncCode==0x02)//文本控件，主要是判断设定的值
	{
		if((UART3RxBuf[3]==0x00)&&(UART3RxBuf[4]==0x01))//改反馈信号只在画面二
		{
				if((UART3RxBuf[5]==0x00)&&(UART3RxBuf[6]==0x09))//设定的温度值
				{
					g_Pdu.RegHold[0]=(UART3RxBuf[8]-0x30)*10+(UART3RxBuf[9]-0x30);
				}
				else if((UART3RxBuf[5]==0x00)&&(UART3RxBuf[6]==0x0A))//设定的CO2值
				{
					if(UART3RxCount==15)//2位数
					  g_Pdu.RegHold[3]=(UART3RxBuf[8]-0x30)*10+(UART3RxBuf[9]-0x30);
					else if(UART3RxCount==16)//3位数
						g_Pdu.RegHold[3]=(UART3RxBuf[8]-0x30)*100+(UART3RxBuf[9]-0x30)*10+(UART3RxBuf[10]-0x30);
					else if(UART3RxCount==17)//4位数
						g_Pdu.RegHold[3]=(UART3RxBuf[8]-0x30)*1000+(UART3RxBuf[9]-0x30)*100+(UART3RxBuf[10]-0x30)*10+(UART3RxBuf[11]-0x30);
				}
				else if((UART3RxBuf[5]==0x00)&&(UART3RxBuf[6]==0x0B))//设定的PM2.5
				{
					if(UART3RxCount==15)//2位数
					  g_Pdu.RegHold[1]=(UART3RxBuf[8]-0x30)*10+(UART3RxBuf[9]-0x30);
					else if(UART3RxCount==16)//3位数
						g_Pdu.RegHold[1]=(UART3RxBuf[8]-0x30)*100+(UART3RxBuf[9]-0x30)*10+(UART3RxBuf[10]-0x30);
					else if(UART3RxCount==17)//4位数
						g_Pdu.RegHold[1]=(UART3RxBuf[8]-0x30)*1000+(UART3RxBuf[9]-0x30)*100+(UART3RxBuf[10]-0x30)*10+(UART3RxBuf[11]-0x30);
				}
				else if((UART3RxBuf[5]==0x00)&&(UART3RxBuf[6]==0x0C))//设定的VOC
				{
					if(UART3RxCount==15)//2位数
					  g_Pdu.RegHold[4]=(UART3RxBuf[8]-0x30)*10+(UART3RxBuf[9]-0x30);
					else if(UART3RxCount==16)//3位数
						g_Pdu.RegHold[4]=(UART3RxBuf[8]-0x30)*100+(UART3RxBuf[9]-0x30)*10+(UART3RxBuf[10]-0x30);
					else if(UART3RxCount==17)//4位数
						g_Pdu.RegHold[4]=(UART3RxBuf[8]-0x30)*1000+(UART3RxBuf[9]-0x30)*100+(UART3RxBuf[10]-0x30)*10+(UART3RxBuf[11]-0x30);
				}	
		}
	}
	if(ErrCode!=0)	                    //有错误存在
	{
	  return 0;
	}
	else
	{
	  return FuncCode;
	}	
}
