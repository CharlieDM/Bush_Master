#ifndef __HAL_UART3_H
#define __HAL_UART3_H

#define	USART3_FIFO_EN	1

#if USART3_FIFO_EN == 1
	#define USART3_BAUD			  9600
#endif

void Hal_Usart3_Init(void);

#endif
