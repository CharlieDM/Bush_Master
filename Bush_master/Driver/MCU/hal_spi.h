#ifndef __HAL_SPI_H
#define __HAL_SPI_H
#include "stm32f10x.h"

#define SOFT_SPI

/* 定义SPI总线的 GPIO端口 */
#define RCC_NSS 	RCC_APB2Periph_GPIOB
#define PORT_NSS	GPIOB
#define PIN_NSS		GPIO_Pin_15

#define RCC_SCK 	RCC_APB2Periph_GPIOB
#define PORT_SCK	GPIOB
#define PIN_SCK		GPIO_Pin_14

#define RCC_MISO 	RCC_APB2Periph_GPIOB
#define PORT_MISO	GPIOB
#define PIN_MISO	GPIO_Pin_12

#define RCC_MOSI 	RCC_APB2Periph_GPIOB
#define PORT_MOSI	GPIOB
#define PIN_MOSI	GPIO_Pin_13

//软件控制片选脚
#define SPI1_CS_LOW        (PORT_NSS->BRR = PIN_NSS)
#define SPI1_CS_HIGH       (PORT_NSS->BSRR  = PIN_NSS)

#ifdef SOFT_SPI		/* 软件SPI */
	#define SCK_0()		PORT_SCK->BRR = PIN_SCK
	#define SCK_1()		PORT_SCK->BSRR = PIN_SCK

	#define MOSI_0()	PORT_MOSI->BRR = PIN_MOSI
	#define MOSI_1()	PORT_MOSI->BSRR = PIN_MOSI

	#define MISO_IS_HIGH()	(GPIO_ReadInputDataBit(PORT_MISO, PIN_MISO) == Bit_SET)
#endif

void    Hal_Spi_Init(void);

#endif
