
#ifndef _CORE_HAL_H_
#define _CORE_HAL_H_

#include "type_def.h"
#include "stm32f10x.h"

typedef struct Device
{   
	struct Gpio
	{
		void (*DoutOn)(GPIO_TypeDef _port,uint16 _pin);
		void (*DoutOff)(GPIO_TypeDef _port,uint16 _pin);
		uint16 (*Din)(GPIO_TypeDef *_port);
	}Gpio;
	
	struct Usart3
	{
		void (*Register)(void *rxdFucn);
		void (*Send)(uint8 *TBuf, uint16 _usLen);
	}Usart3;		    
	
	struct Usart5
	{
		void (*Register)(void *rxdFucn);
		void (*Send)(uint8 *TBuf, uint16 _usLen);
	}Usart5;		    
   
	struct Spi
	{
		void (*ReadWrite)(uint8 len,uint8 *wbuf, uint8 * rbuf);
	}Spi;	
	
	struct Systick
	{
		void (*Register)(uint8 timer,void(*Func)());
		void (*Run)(void);
	}Systick;
	
}DeviceStruct;


extern DeviceStruct Device;

uint8 HAL_Init(void);

#endif /* _MOE_HAL_H_ */

/* End of file */

