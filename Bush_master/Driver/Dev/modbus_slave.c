
#include "modproto.h"
#include "modbus_slave.h"
#include "modfunc_slave.h"
#include "core_hal.h"
#include "hal_systick.h"
#include "queue.h"
#include "crc.h"


static xMODFuncHandler xFuncHandler[] =
{
	{MOD_FUNC_READ_MULTIPLE_REGISTER,eModSlaveReadRegister},
	{MOD_FUNC_WRITE_SINGLE_REGISTER,eModSlaveWriteSingleRegister},
	{MOD_FUNC_WRITE_MULTIPLE_REGISTERS,eModSlaveWriteMultiRegister}
};

static uint8 sg_au8Queue[MOD_SLAVE_SIZE_MAX] = {0};
static T_QUEUE_INFO sg_tQueue={sg_au8Queue,0,0,0,MOD_SLAVE_SIZE_MAX};
static uint8 ucFrameBuf[MOD_SLAVE_SIZE_MAX] = {0};
static eMODState eEvent = MOD_INIT;

static void Receive(uint8 data)
{
	Queue_Push(&sg_tQueue,data);
}

static uint8 ModFrameReceive(uint8 *uclen)
{
	uint8 ucdata;
	uint8 ucpos = 0;

	if(sg_tQueue.u8Cnt < MOD_SLAVE_SIZE_MIN) return MOD_NOT_YET;
	
	while( sg_tQueue.u8Cnt > 0 )
	{
		Queue_Pop(&sg_tQueue1,&ucdata);		
		if(data != MOD_ADDRESS_SLAVE && ucpos == 0 ) continue; 	
	
		if(ucpos < MOD_SLAVE_SIZE_MIN)
		{
			ucFrameBuf[ucpos++] = ucdata;
		}

		switch(ucdata && ucpos == 2)
		{
			case MOD_FUNC_READ_MULTIPLE_REGISTER:
				Queue_Pop(&sg_tQueue1,&ucdata);
				ucFrameBuf[ucpos++] = ucdata;
				uclen = ucdata+5;
				break;
			
			case MOD_FUNC_WRITE_SINGLE_REGISTER:
				
				uclen = 6;
				break;
			
			case MOD_FUNC_WRITE_MULTIPLE_REGISTERS:
				uclen = 6;
				break;
			
			default:	
				ucpos = 0;
				break;
		}

		if( uclen > 0 && ucpos > 0 && ucpos >= uclen ) break;
	}

	if(uclen >= MOD_SLAVE_SIZE_MIN && usMBCRC16(ucFrameBuf,uclen) == 0 ) return MOD_EXEC;
	
	return MOD_ERROR;
}

static void ModbusPoll()
{
	static uint8 ucLen = 0;
	static uint8 ucFunctionCode = 0;
	static eMODException eException;
	static uint8 eStatus = 0;
	
	uint8 i = 0;
	
	switch(eEvent)
	{
		case MOD_INIT:
			
			break;
			
		case MOD_FRAME:
			eStatus = ModFrameReceive();
			break;
		
		default:
			break;
	}
	
	switch(eStatus)
	{
		case MOD_EXEC:
			ucFunctionCode = ucFrameBuf[MOD_FUNCTION_CODE];
			for( i=0; i<MOD_FUNC_HANDLERS_MAX;i++ )
			{
				if( xFuncHandler[i].ucFunctionCode == 0 )
                {
                    break;
                }
                else if( xFuncHandler[i].ucFunctionCode == ucFunctionCode )
                {
                    eException = xFuncHandler[i].pxHandler( ucFrameBuf, ucLen );
                    break;
                }
			}
			
			break;
			
		case MOD_ERROR:
			
			break;
			
		default:
			break;

	}
}

void ModbusHostInit()
{
	Device.Usart3.Register(Receive);
	Device.Systick.Register(100,ModPoll);
}
