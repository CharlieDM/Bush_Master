#ifndef __HAL_UART5_H
#define __HAL_UART5_H

#define	UART5_FIFO_EN	1

/* RS485Ð¾Æ¬·¢ËÍÊ¹ÄÜGPIO, PD1 */
#define RCC_RS485_TXEN 	 RCC_APB2Periph_GPIOD
#define PORT_RS485_TXEN  GPIOD
#define PIN_RS485_TXEN	 GPIO_Pin_1

#define RS485_RX_EN()	   PORT_RS485_TXEN->BRR = PIN_RS485_TXEN
#define RS485_TX_EN()	   PORT_RS485_TXEN->BSRR = PIN_RS485_TXEN

#if UART5_FIFO_EN == 1
	#define UART5_BAUD			  9600
#endif

void Hal_Uart5_Init(void);

#endif
