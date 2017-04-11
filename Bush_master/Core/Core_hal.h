
#ifndef _CORE_HAL_H_
#define _CORE_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "type_def.h"
	
typedef struct Device
{     
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
		void (*WriteLow)(uint8 data);
		uint8 (*ReadLow)(void);
		void (*WriteHigh)(uint8 data);
		uint8 (*ReadHigh)(void);
	}Spi;	
	
	struct Systick
	{
		void (*Register)(uint8 timer,void(*Func)());
		void (*Run)(void);
	}Systick;
	
}DeviceStruct;


extern DeviceStruct Device;

uint8 HAL_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* _MOE_HAL_H_ */

/* End of file */

