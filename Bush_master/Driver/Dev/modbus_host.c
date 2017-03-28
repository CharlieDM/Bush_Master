
#include "modbus_host.h"
#include "core_hal.h"
#include "queue.h"
#include "modproto.h"
#include "hal_systick.h"

static eMODException Func( uint8 * pucFrame, uint16 * pusLength ){return MOD_EX_NONE;}

static xMODFuncHandler xFuncHandler[] =
{
	{MOD_FUNC_READ_MULTIPLE_REGISTER,Func},
	{MOD_FUNC_WRITE_SINGLE_REGISTER,Func},
	{MOD_FUNC_WRITE_MULTIPLE_REGISTERS,Func}
};

static uint8 sg_au8Queue[MOD_SIZE_MAX] = {0};
static T_QUEUE_INFO sg_tQueue1={sg_au8Queue,0,0,0,0};

static void Receive(uint8 data)
{
	Queue_Push(&sg_tQueue1,data);
}

static uint8 ModFrameReceive()
{
	uint8 data;
	uint8 len = 0;
	while( sg_tQueue1.u8Cnt > MOD_SIZE_MIN )
	{
		Queue_Pop(&sg_tQueue1,&data);
		
		if(data != MOD_ADDRESS_HOST && len == 0 ) continue; 
		
		switch(data)
		{
			case MOD_FUNC_READ_MULTIPLE_REGISTER:
				
				break;
			
			case MOD_FUNC_WRITE_SINGLE_REGISTER:
				
				break;
			
			case MOD_FUNC_WRITE_MULTIPLE_REGISTERS:
				
				break;
			
			default:				
				break;
		}
		
	}
	return 0;
}

static void ModPoll()
{
	
}

void ModbusHostInit()
{
	Device.Usart3.Register(Receive);
	Device.Systick.Register(100,ModPoll);
}
