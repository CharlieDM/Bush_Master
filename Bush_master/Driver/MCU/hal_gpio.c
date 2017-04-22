
#include "hal_gpio.h"
#include "core_hal.h"

static void Hal_GpioOutOn(GPIO_TypeDef *_port,uint16 _pin);
static void Hal_GpioOutOff(GPIO_TypeDef *_port,uint16 _pin);
static uint16 Hal_GpioRead(GPIO_TypeDef *_port);

void Hal_GpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* One Group OUT */
	RCC_APB2PeriphClockCmd(RCC_DOUT_GROUP1, ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;	 
	GPIO_InitStructure.GPIO_Pin   = ELECTRIC_HEAT0|ELECTRIC_HEAT1|AIR_ELECTRIC_HEAT\
									|XHAUST_AIR|FRESH_AIR|LOW_CYCLE|MID_CYCLE|HIGH_CYCLE;
	GPIO_Init(GPIO_PORT_DOUT1, &GPIO_InitStructure);
		
	/* Two Group OUT */
	RCC_APB2PeriphClockCmd(RCC_DOUT_GROUP2, ENABLE);
	GPIO_InitStructure.GPIO_Pin   = RESERVED2|RESERVED1|EXHAUST_AIR_NEG|EXHAUST_AIR_POS\
									|FRESH_AIR_NEG|FRESH_AIR_POS;
	GPIO_Init(GPIO_PORT_DOUT2, &GPIO_InitStructure);

	/* Three Group OUT */
	RCC_APB2PeriphClockCmd(RCC_DOUT_GROUP3, ENABLE);
	GPIO_InitStructure.GPIO_Pin   = HUMIDIFIER|ELECTRIC_HEAT3;
	GPIO_Init(GPIO_PORT_DOUT3, &GPIO_InitStructure);

	/* One Group In */
	RCC_APB2PeriphClockCmd(RCC_DIN_GROUP1, ENABLE);
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;	 
 	GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_IN0|GPIO_PIN_IN6|GPIO_PIN_IN7;
 	GPIO_Init(GPIO_PORT_DIN_GROUP1, &GPIO_InitStructure);

	/* Two Group IN */
	RCC_APB2PeriphClockCmd(RCC_DIN_GROUP2, ENABLE);
 	GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_IN1|GPIO_PIN_IN2|GPIO_PIN_IN3
									|GPIO_PIN_IN4|GPIO_PIN_IN5;
 	GPIO_Init(GPIO_PORT_DIN_GROUP1, &GPIO_InitStructure);

	/* Three Group IN */
	RCC_APB2PeriphClockCmd(RCC_DIN_GROUP3, ENABLE);	 
 	GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_IN8|GPIO_PIN_IN9|GPIO_PIN_IN10;
 	GPIO_Init(GPIO_PORT_DIN_GROUP1, &GPIO_InitStructure);	

	/* Four Group IN */
	RCC_APB2PeriphClockCmd(RCC_DIN_GROUP3, ENABLE);	 
 	GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_IN11;
 	GPIO_Init(GPIO_PORT_DIN_GROUP1, &GPIO_InitStructure);	

	Device.Gpio.DoutOn = Hal_GpioOutOn;
	Device.Gpio.DoutOff = Hal_GpioOutOff;
	Device.Gpio.Din = Hal_GpioRead;
}

static void Hal_GpioOutOn(GPIO_TypeDef *_port,uint16 _pin)
{
	_port->BSRR = _pin;
}

static void Hal_GpioOutOff(GPIO_TypeDef *_port,uint16 _pin)
{
	_port->BRR = _pin;
}

static uint16 Hal_GpioRead(GPIO_TypeDef *_port)
{
	return GPIO_ReadInputData(_port);
}







