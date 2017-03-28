#include "bsp_global.h"
#include <string.h>
#include "Comm.h"
#include "crc.h"

uint8_t UART3RxBuf[RXBUFLEN];	         //���ջ�����
uint8_t UART3RxCount=0;
uint8_t UART3TxBuf[TXBUFLEN];	           //���ͻ�����
uint8_t UART3RxBuf[TXBUFLEN];	           //���ͻ�����
uint8_t UART5RxCount=0;
uint8_t UART5TxBuf[TXBUFLEN];	           //���ͻ�����
uint8_t UART5RxBuf[TXBUFLEN];	           //���ͻ�����
struct _PDU	g_Pdu;
/**************************************************************
��������Comm_MbMtInit()
���룺
�����
���ܣ�  
***************************************************************/
void Comm_MbMtInit(void)
{	
	
}


/**************************************************************
��������Comm_MbSl_rep()
���룺
������ɹ��򷵻ع����룬0����ʧ��
���ܣ��������ܵ���Э��������н��룬��Ϊ
***************************************************************/

uint16_t Comm_Explain(void)
{
	uint8_t  FuncCode;
	uint8_t  ErrCode=0;
//DecodeSentdat�����е�ǰMBTXBUFLEN����������Ϊ��
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
				  FuncCode=0x01;//��ť�ؼ�
			 else if(UART3RxBuf[7]==0x11)
				  FuncCode=0x02;//�ı��ؼ�
			 else if(UART3RxBuf[7]==0x12)
				  FuncCode=0x03;//�������ؼ�
			 else if(UART3RxBuf[7]==0x13)
				  FuncCode=0x04;//����ؼ�
			 else if(UART3RxBuf[7]==0x14)
				  FuncCode=0x05;//�Ǳ�ؼ�	 
	}
	if(FuncCode==0x01)//�жϾ���ؼ�
	{
		 if((UART3RxBuf[3]==0x00)&&(UART3RxBuf[4]==0x00))
		 {
			 if((UART3RxBuf[5]==0x00)&&(UART3RxBuf[6]==0x54))//����0�ĵ�84���ؼ������ذ�ť
			 {
				 if(UART3RxBuf[9]==0x01)
					 g_Pdu.BitCoil[0]|=0x10;//���ذ�ť
				 else
					 g_Pdu.BitCoil[0]&=~0x10;
			 }
		 }
		 else if((UART3RxBuf[3]==0x00)&&(UART3RxBuf[4]==0x01))
		 {
			 if((UART3RxBuf[5]==0x00)&&(UART3RxBuf[6]==0x29))//����0�ĵ�84���ؼ������ذ�ť
			 {
				 if(UART3RxBuf[9]==0x01)
					 g_Pdu.BitCoil[0]|=0x10;
				 else
					 g_Pdu.BitCoil[0]&=~0x10;
			 }
			 else if((UART3RxBuf[5]==0x00)&&(UART3RxBuf[6]==0x03))//�Զ�ģʽ,�ֶ�ģʽ
			 {
				 if(UART3RxBuf[9]==0x01)
				  g_Pdu.BitCoil[0]&=~0x20;
				 else 
					 g_Pdu.BitCoil[0]|=0x20;
			 }
			 else if ((UART3RxBuf[5]==0x00)&&(UART3RxBuf[6]==0x04))//�ж��ֶ����������
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
			 else if ((UART3RxBuf[5]==0x00)&&(UART3RxBuf[6]==0x05))//�ж��ֶ����������
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
			 else if((UART3RxBuf[5]==0x00)&&((UART3RxBuf[6]==0x0E)||(UART3RxBuf[6]==0x0F)|(UART3RxBuf[6]==0x10)))//���ٵ���
			 {
			    //�����ơ���������������������������������
				 if(UART3RxBuf[6]==0x0E)
				 {
					  g_Pdu.BitCoil[0]&=~0x02;
					  g_Pdu.BitCoil[0]&=~0x04;
					  g_Pdu.BitCoil[0]|=0x01;//�͵��ٶ�
				 }
				 else if(UART3RxBuf[6]==0x0f)
				 {
					  g_Pdu.BitCoil[0]&=~0x01;
					  g_Pdu.BitCoil[0]&=~0x04;
					  g_Pdu.BitCoil[0]|=0x02;//�е��ٶ�
				 }
				 else if(UART3RxBuf[6]==0x10)
				 {
					  g_Pdu.BitCoil[0]&=~0x01;
					  g_Pdu.BitCoil[0]&=~0x02;
					  g_Pdu.BitCoil[0]|=0x04;//�ߵ��ٶ�
				 }
			 }
			 else if((UART3RxBuf[5]==0x00)&&(UART3RxBuf[6]==0x0D))//��ʪģʽ
			 {
				  if(UART3RxBuf[9]==0x01)
				   g_Pdu.BitCoil[0]|=0x40;
				  else
					 g_Pdu.BitCoil[0]&=~0x40;
				 ;
			 }
			 else if((UART3RxBuf[5]==0x00)&&(UART3RxBuf[6]==0x08))//��ǿ����
			 {
				 if(UART3RxBuf[9]==0x01)
				   g_Pdu.BitCoil[0]|=0x80;
				 else
					 g_Pdu.BitCoil[0]&=~0x80;
			 }
			 else if((UART3RxBuf[5]==0x00)&&(UART3RxBuf[6]==0x07))//�·�ģʽ
			 {
				  if(UART3RxBuf[9]==0x01)
				   g_Pdu.BitCoil[0]|=0x100;
				  else
					 g_Pdu.BitCoil[0]&=~0x100;
			 }
			 else if((UART3RxBuf[5]==0x00)&&(UART3RxBuf[6]==0x06))//����ģʽ
			 {
				  if(UART3RxBuf[9]==0x01)
				   g_Pdu.BitCoil[0]|=0x400;
				  else
					 g_Pdu.BitCoil[0]&=~0x400;
			 } 
			 
		  }
	 }
	else if(FuncCode==0x02)//�ı��ؼ�����Ҫ���ж��趨��ֵ
	{
		if((UART3RxBuf[3]==0x00)&&(UART3RxBuf[4]==0x01))//�ķ����ź�ֻ�ڻ����
		{
				if((UART3RxBuf[5]==0x00)&&(UART3RxBuf[6]==0x09))//�趨���¶�ֵ
				{
					g_Pdu.RegHold[0]=(UART3RxBuf[8]-0x30)*10+(UART3RxBuf[9]-0x30);
				}
				else if((UART3RxBuf[5]==0x00)&&(UART3RxBuf[6]==0x0A))//�趨��CO2ֵ
				{
					if(UART3RxCount==15)//2λ��
					  g_Pdu.RegHold[3]=(UART3RxBuf[8]-0x30)*10+(UART3RxBuf[9]-0x30);
					else if(UART3RxCount==16)//3λ��
						g_Pdu.RegHold[3]=(UART3RxBuf[8]-0x30)*100+(UART3RxBuf[9]-0x30)*10+(UART3RxBuf[10]-0x30);
					else if(UART3RxCount==17)//4λ��
						g_Pdu.RegHold[3]=(UART3RxBuf[8]-0x30)*1000+(UART3RxBuf[9]-0x30)*100+(UART3RxBuf[10]-0x30)*10+(UART3RxBuf[11]-0x30);
				}
				else if((UART3RxBuf[5]==0x00)&&(UART3RxBuf[6]==0x0B))//�趨��PM2.5
				{
					if(UART3RxCount==15)//2λ��
					  g_Pdu.RegHold[1]=(UART3RxBuf[8]-0x30)*10+(UART3RxBuf[9]-0x30);
					else if(UART3RxCount==16)//3λ��
						g_Pdu.RegHold[1]=(UART3RxBuf[8]-0x30)*100+(UART3RxBuf[9]-0x30)*10+(UART3RxBuf[10]-0x30);
					else if(UART3RxCount==17)//4λ��
						g_Pdu.RegHold[1]=(UART3RxBuf[8]-0x30)*1000+(UART3RxBuf[9]-0x30)*100+(UART3RxBuf[10]-0x30)*10+(UART3RxBuf[11]-0x30);
				}
				else if((UART3RxBuf[5]==0x00)&&(UART3RxBuf[6]==0x0C))//�趨��VOC
				{
					if(UART3RxCount==15)//2λ��
					  g_Pdu.RegHold[4]=(UART3RxBuf[8]-0x30)*10+(UART3RxBuf[9]-0x30);
					else if(UART3RxCount==16)//3λ��
						g_Pdu.RegHold[4]=(UART3RxBuf[8]-0x30)*100+(UART3RxBuf[9]-0x30)*10+(UART3RxBuf[10]-0x30);
					else if(UART3RxCount==17)//4λ��
						g_Pdu.RegHold[4]=(UART3RxBuf[8]-0x30)*1000+(UART3RxBuf[9]-0x30)*100+(UART3RxBuf[10]-0x30)*10+(UART3RxBuf[11]-0x30);
				}	
		}
	}
	if(ErrCode!=0)	                    //�д������
	{
	  return 0;
	}
	else
	{
	  return FuncCode;
	}	
}
