#ifndef _MODBUS_H_
#define _MODBUS_H_
#include "bsp_global.h"

#define COMM_BUTTON      0x01;
#define COMM_TEXT        0x02;
#define COMM_BAR         0x03;

#define	RXBUFLEN			(300)		//���ּĴ���������ȡ���������
#define	TXBUFLEN			(300)		//���ּĴ�������д����������

	                        
//����ṹ���Ǻ���ʾ�йأ����Ǻ�MODBUS��ص�
struct _PDU
{
	uint16_t BitInp[80];		          //��ɢ��������ֻ��
	uint16_t BitCoil[80];            //��Ȧ����д
	uint16_t RegInp[80];	            //����Ĵ�����ֻ��
	uint16_t RegHold[80];            //���ּĴ�������д
};

extern struct _PDU	 g_Pdu;
extern uint8_t  UART3RxBuf[RXBUFLEN];	         //���ջ�����
extern uint8_t  UART3TxBuf[TXBUFLEN];	       //���ͻ�����
extern uint8_t  UART3RxCount;
extern uint8_t  UART5RxCount;
extern uint8_t  UART5TxBuf[TXBUFLEN];	       //���ͻ�����
extern uint8_t  UART5RxBuf[RXBUFLEN];	       //���ͻ�����
/********************************MODBUS*********************************************/
extern void      Comm_MbMtInit(void);
extern uint16_t  Comm_Explain(void);

#endif
