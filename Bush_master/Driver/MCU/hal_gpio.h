#ifndef __HAL_GPIO_H
#define __HAL_GPIO_H

#include "stm32f10x.h"

/* RCC GPIO OUT */
#define RCC_DOUT_GROUP1 	RCC_APB2Periph_GPIOE
#define RCC_DOUT_GROUP2 	RCC_APB2Periph_GPIOD
#define RCC_DOUT_GROUP3 	RCC_APB2Periph_GPIOC

/* One Group OUT */
#define GPIO_PORT_DOUT1     GPIOE

#define ELECTRIC_HEAT0	    GPIO_Pin_8
#define ELECTRIC_HEAT1	    GPIO_Pin_9
#define AIR_ELECTRIC_HEAT   GPIO_Pin_10
#define XHAUST_AIR		    GPIO_Pin_11
#define FRESH_AIR		    GPIO_Pin_12
#define LOW_CYCLE		    GPIO_Pin_13
#define MID_CYCLE		    GPIO_Pin_14
#define HIGH_CYCLE		    GPIO_Pin_15

/* Three Group OUT */
#define GPIO_PORT_DOUT2     GPIOD

#define RESERVED2		    GPIO_Pin_10
#define RESERVED1		    GPIO_Pin_11
#define EXHAUST_AIR_NEG	    GPIO_Pin_12
#define EXHAUST_AIR_POS	    GPIO_Pin_13
#define FRESH_AIR_NEG		GPIO_Pin_14
#define FRESH_AIR_POS	    GPIO_Pin_15

/* Three Group OUT */
#define GPIO_PORT_DOUT3    	GPIOC

#define HUMIDIFIER	    	GPIO_Pin_6
#define ELECTRIC_HEAT3	    GPIO_Pin_7

/* RCC GPIO IN */
#define RCC_DIN_GROUP1	         	RCC_APB2Periph_GPIOC
#define RCC_DIN_GROUP2	         	RCC_APB2Periph_GPIOA
#define RCC_DIN_GROUP3	         	RCC_APB2Periph_GPIOB
#define RCC_DIN_GROUP4	         	RCC_APB2Periph_GPIOE

/* One Group IN */
#define GPIO_PORT_DIN_GROUP1     	GPIOC
#define GPIO_PIN_IN0	           	GPIO_Pin_3
#define GPIO_PIN_IN6	           	GPIO_Pin_4
#define GPIO_PIN_IN7	           	GPIO_Pin_5

/* Two Group IN */
#define GPIO_PORT_DIN_GROUP2     	GPIOA
#define GPIO_PIN_IN1	           	GPIO_Pin_0
#define GPIO_PIN_IN2	           	GPIO_Pin_1
#define GPIO_PIN_IN3	           	GPIO_Pin_5
#define GPIO_PIN_IN4	           	GPIO_Pin_6
#define GPIO_PIN_IN5	           	GPIO_Pin_7

/* Three Group IN */
#define GPIO_PORT_DIN_GROUP3     	GPIOB
#define GPIO_PIN_IN8	           	GPIO_Pin_0
#define GPIO_PIN_IN9	           	GPIO_Pin_1
#define GPIO_PIN_IN10	           	GPIO_Pin_2

/* Four Group IN */
#define GPIO_PORT_DIN_GROUP4     	GPIOE
#define GPIO_PIN_IN11	           	GPIO_Pin_7


void Hal_GpioInit(void);

#endif
