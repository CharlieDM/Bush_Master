#ifndef __IO_H
#define __IO_H

#include "hal_gpio.h"
#include "core_hal.h"

/* ----------------------- Macro definitions ---------------------------------*/
#define HIGH_CYCLE_ON          	Device.Gpio.DoutOn(GPIO_PORT_DOUT1,HIGH_CYCLE)
#define HIGH_CYCLE_OFF         	Device.Gpio.DoutOff(GPIO_PORT_DOUT1,HIGH_CYCLE)

#define MID_CYCLE_ON           	Device.Gpio.DoutOn(GPIO_PORT_DOUT1,MID_CYCLE) 
#define MID_CYCLE_OFF          	Device.Gpio.DoutOff(GPIO_PORT_DOUT1,MID_CYCLE)

#define LOW_CYCLE_ON          	Device.Gpio.DoutOn(GPIO_PORT_DOUT1,LOW_CYCLE) 
#define LOW_CYCLE_OFF          	Device.Gpio.DoutOff(GPIO_PORT_DOUT1,LOW_CYCLE) 

#define FRESH_AIR_ON           	Device.Gpio.DoutOn(GPIO_PORT_DOUT1,FRESH_AIR) 
#define FRESH_AIR_OFF          	Device.Gpio.DoutOff(GPIO_PORT_DOUT1,FRESH_AIR) 

#define EXHAUST_AIR_ON         	Device.Gpio.DoutOn(GPIO_PORT_DOUT1,XHAUST_AIR) 
#define EXHAUST_AIR_OFF        	Device.Gpio.DoutOff(GPIO_PORT_DOUT1,XHAUST_AIR) 

#define AIR_ELECTRIC_HEAT_ON  	Device.Gpio.DoutOn(GPIO_PORT_DOUT1,AIR_ELECTRIC_HEAT) 
#define AIR_ELECTRIC_HEAT_OFF  	Device.Gpio.DoutOff(GPIO_PORT_DOUT1,AIR_ELECTRIC_HEAT) 

#define ELECTRIC_HEAT1_ON     	Device.Gpio.DoutOn(GPIO_PORT_DOUT1,ELECTRIC_HEAT1) 
#define ELECTRIC_HEAT1_OFF     	Device.Gpio.DoutOff(GPIO_PORT_DOUT1,ELECTRIC_HEAT1) 

#define ELECTRIC_HEAT2_ON      	Device.Gpio.DoutOn(GPIO_PORT_DOUT1,ELECTRIC_HEAT0) 
#define ELECTRIC_HEAT2_OFF     	Device.Gpio.DoutOff(GPIO_PORT_DOUT1,ELECTRIC_HEAT0) 

#define ELECTRIC_HEAT3_ON      	Device.Gpio.DoutOn(GPIO_PORT_DOUT3,ELECTRIC_HEAT3) 
#define ELECTRIC_HEAT3_OFF     	Device.Gpio.DoutOff(GPIO_PORT_DOUT3,ELECTRIC_HEAT3) 

#define HUMIDIFIER_ON          	Device.Gpio.DoutOn(GPIO_PORT_DOUT3,HUMIDIFIER) 
#define HUMIDIFIER_OFF         	Device.Gpio.DoutOff(GPIO_PORT_DOUT3,HUMIDIFIER) 

#define FRESH_AIR_POS_ON       	Device.Gpio.DoutOn(GPIO_PORT_DOUT2,FRESH_AIR_POS) 
#define FRESH_AIR_POS_OFF      	Device.Gpio.DoutOff(GPIO_PORT_DOUT2,FRESH_AIR_POS) 

#define FRESH_AIR_NEG_ON       	Device.Gpio.DoutOn(GPIO_PORT_DOUT2,FRESH_AIR_NEG) 
#define FRESH_AIR_NEG_OFF      	Device.Gpio.DoutOff(GPIO_PORT_DOUT2,FRESH_AIR_NEG) 

#define EXHAUST_AIR_POS_ON     	Device.Gpio.DoutOn(GPIO_PORT_DOUT2,EXHAUST_AIR_POS) 
#define EXHAUST_AIR_POS_OFF    	Device.Gpio.DoutOff(GPIO_PORT_DOUT2,EXHAUST_AIR_POS) 

#define EXHAUST_AIR_NEG_ON     	Device.Gpio.DoutOn(GPIO_PORT_DOUT2,EXHAUST_AIR_NEG) 
#define EXHAUST_AIR_NEG_OFF    	Device.Gpio.DoutOff(GPIO_PORT_DOUT2,EXHAUST_AIR_NEG) 

#define DRAIN_PUMP_ON           Device.Gpio.DoutOn(GPIO_PORT_DOUT2,DRAIN_PUMP) 
#define DRAIN_PUMP_OFF          Device.Gpio.DoutOff(GPIO_PORT_DOUT2,DRAIN_PUMP) 

#define DHUMIDIFIER_ON          Device.Gpio.DoutOn(GPIO_PORT_DOUT2,RESERVED2) 
#define DHUMIDIFIER_OFF         Device.Gpio.DoutOff(GPIO_PORT_DOUT2,RESERVED2) 

/* ----------------------- Functions ---------------------------------*/
uint16 ReadIO(void);

#endif
