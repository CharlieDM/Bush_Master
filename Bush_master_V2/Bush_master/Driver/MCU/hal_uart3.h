#ifndef __HAL_UART3_H
#define __HAL_UART3_H

#define	USART3_FIFO_EN	1

#if USART3_FIFO_EN == 1
	#define USART3_BAUD			  9600
#endif
#define RCC_RS485 	 	RCC_APB2Periph_GPIOA
#define PORT_RS485  	GPIOA
#define PIN_RS485		GPIO_Pin_10

#define RS485_RX()	   PORT_RS485->BRR = PIN_RS485
#define RS485_TX()	   PORT_RS485->BSRR = PIN_RS485

void Hal_Usart3_Init(void);

#endif
