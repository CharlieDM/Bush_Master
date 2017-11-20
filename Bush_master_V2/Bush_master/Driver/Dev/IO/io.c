
#include "io.h"

/******************************************************************************
* Name       : uint16 ReadIO(void)
* Function   : read io state
******************************************************************************/
uint16 ReadIO(void)
{
	uint16 ucTemp = 0,ucState = 0;
	
	/* 3 4 5 -- 0 6 7 C */
	ucTemp = Device.Gpio.Din(GPIO_PORT_DIN_GROUP1);
	if(ucTemp & (0x1<<3)) ucState |= 0x01;
	if(ucTemp & (0x1<<4)) ucState |= 0x1<<6;
	if(ucTemp & (0x1<<5)) ucState |= 0x1<<7;

	/* 0 1 5 6 7 -- 1 2 3 4 5 A*/
	ucTemp = Device.Gpio.Din(GPIO_PORT_DIN_GROUP2);
	if(ucTemp & (0x1<<0)) ucState |= 0x1<<1;
	if(ucTemp & (0x1<<1)) ucState |= 0x1<<2;
	if(ucTemp & (0x1<<5)) ucState |= 0x1<<3;
	if(ucTemp & (0x1<<6)) ucState |= 0x1<<4;
    if(ucTemp & (0x1<<7)) ucState |= 0x1<<5;
	/* 0 1 2 -- 8 9 10 B*/
	ucTemp = Device.Gpio.Din(GPIO_PORT_DIN_GROUP3);
	if(ucTemp & (0x1<<0)) ucState |= 0x1<<8;
	if(ucTemp & (0x1<<1)) ucState |= 0x1<<9;
	if(ucTemp & (0x1<<2)) ucState |= 0x1<<10;

	/* 7 -- 11 E*/
	ucTemp = Device.Gpio.Din(GPIO_PORT_DIN_GROUP4);
	if(ucTemp & (0x1<<7)) ucState |= 0x1<<11;

	return ucState;
}

