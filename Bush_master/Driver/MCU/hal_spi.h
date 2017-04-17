#ifndef __HAL_SPI_H
#define __HAL_SPI_H
#include "stm32f10x.h"

/* ----------------------- Defines ------------------------------------------*/
#define SPI1_CPOL 			( 0x01 )

#define RCC_SPI1 	      	RCC_APB2Periph_GPIOB
#define PORT_SPI1	      	GPIOB
#define PIN_NSS1		    GPIO_Pin_15
#define PIN_SCK1		    GPIO_Pin_14
#define PIN_MOSI1  	    	GPIO_Pin_13
#define PIN_MISO1  	    	GPIO_Pin_12

#define SPI1_NSS_0()		PORT_SPI1->BRR  = PIN_NSS1
#define SPI1_NSS_1()		PORT_SPI1->BSRR = PIN_NSS1
#define SPI1_SCK_0()		PORT_SPI1->BRR  = PIN_SCK1
#define SPI1_SCK_1()		PORT_SPI1->BSRR = PIN_SCK1
#define SPI1_MOSI_0()	  	PORT_SPI1->BRR  = PIN_MOSI1
#define SPI1_MOSI_1()	  	PORT_SPI1->BSRR = PIN_MOSI1
	
#define MISO1_IS_HIGH()	(GPIO_ReadInputDataBit(PORT_SPI1, PIN_MISO1) == Bit_SET)

/* ----------------------- Funs ------------------------------------------*/
void    Hal_Spi_Init(void);

#endif
