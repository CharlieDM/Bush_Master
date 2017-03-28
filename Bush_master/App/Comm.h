#ifndef _MODBUS_H_
#define _MODBUS_H_
#include "bsp_global.h"

#define COMM_BUTTON      0x01;
#define COMM_TEXT        0x02;
#define COMM_BAR         0x03;

#define	RXBUFLEN			(300)		//保持寄存器连续读取的最大数量
#define	TXBUFLEN			(300)		//保持寄存器连续写入的最大数量

	                        
//这个结构体是和显示有关，都是和MODBUS相关的
struct _PDU
{
	uint16_t BitInp[80];		          //离散输入量，只读
	uint16_t BitCoil[80];            //线圈，读写
	uint16_t RegInp[80];	            //输入寄存器，只读
	uint16_t RegHold[80];            //保持寄存器，读写
};

extern struct _PDU	 g_Pdu;
extern uint8_t  UART3RxBuf[RXBUFLEN];	         //接收缓冲区
extern uint8_t  UART3TxBuf[TXBUFLEN];	       //发送缓冲区
extern uint8_t  UART3RxCount;
extern uint8_t  UART5RxCount;
extern uint8_t  UART5TxBuf[TXBUFLEN];	       //发送缓冲区
extern uint8_t  UART5RxBuf[RXBUFLEN];	       //发送缓冲区
/********************************MODBUS*********************************************/
extern void      Comm_MbMtInit(void);
extern uint16_t  Comm_Explain(void);

#endif
