
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

static uint16 ucTimes = 0;
static pFunc pfStateFunc = NULL;

static SlaveStruct stSlave =
{

}

static void Receive(uint8 data)
{
	Queue_Push(&sg_tQueue,data);
}

static void ModbusTimeExpire()
{
	if(ucTimes) 
	{
		ucTimes--;	
		if(ucTimes == 0)
		{
			stSlave.uctimeout= 1;
		}
	}
}

static void ModbusDispath(pFunc func)
{
	ucTimes = 0;
	stSlave.uctimeout = 0;
	pfStateFunc = func;
}


static pFunc ModbusRun(void)
{
	return pfStateFunc;
}


static void ModFrameReceive()
{
	uint8 ucdata;
	uint8 ucpos = 0;
	uint8 uclen = 0;

	if(ucTimes == 0)
	{
		ucTimes = stSlave.ustime;
	}

	if(stHost.uctimeout)
	{
		ModbusDispath(ModbusError);
	}
	
	stHost.ucmodstate = MOD_FRAME;
	if(sg_tQueue.u8Cnt < MOD_SLAVE_SIZE_MIN) return;
	
	while( sg_tQueue.u8Cnt > 0 )
	{
		Queue_Pop(&sg_tQueue1,&ucdata);		
		if(data != MOD_ADDRESS_SLAVE && ucpos == 0 ) continue; 	
	
		if(ucpos < MOD_SLAVE_SIZE_MAX)
		{
			ucFrameBuf[ucpos++] = ucdata;
		}

		if( ucpos == 2 )
		{
			switch(ucdata)
			{
				case MOD_FUNC_READ_MULTIPLE_REGISTER:
					Queue_Pop(&sg_tQueue1,&ucdata);
					ucFrameBuf[ucpos++] = ucdata;
					uclen = ucdata+5;
					//stHost.ucmodcmd = MOD_FUNC_READ_MULTIPLE_REGISTER;
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
		}
		
		if( uclen > 0 && ucpos > 0 && ucpos >= uclen ) break;
	}

	if(uclen >= MOD_SLAVE_SIZE_MIN && usMBCRC16(ucFrameBuf,uclen) == 0 )
	{
		stSlave.ucframelen = uclen;
		ModbusDispath(ModbusExec);
	}
	else
	{
		ModbusDispath(ModbusError);
	}	
}

static void ModFrameExec()
{
	uint8 ucFunctionCode = 0;
	eMODException eException;	

	if(ucTimes == 0)
	{
		ucTimes = stSlave.ustime;
	}

	if(stHost.uctimeout)
	{
		ModbusDispath(ModbusError);
	}

	stSlave.ucmodstate = MOD_EXEC;
	ucFunctionCode = ucFrameBuf[MOD_FUNCTION_CODE];
	for( i=0; i<MOD_FUNC_HANDLERS_MAX;i++ )
	{
		if( xFuncHandler[i].ucFunctionCode == 0 )
        {
            break;
        }
        else if( xFuncHandler[i].ucFunctionCode == ucFunctionCode )
        {
            eException = xFuncHandler[i].pxHandler( stSlave.ucbuf, stSlave.ucframelen );
            break;
        }
	}
	
	if(eException)
	{
		ModbusDispath(ModbusError);
	}
}

static void ModbusError()
{
	static uint8 ucerrortimes = 0;
	switch( stSlave.ucmodcmd )
	{
		case MOD_FRAME:
			if(ucerrortimes >= 5)
			{
				stSlave.ucerro |= 0x01;
				ucerrortimes = 0;
			}
			break;

		case MOD_EXEC:
			if(ucerrortimes >= 5)
			{
				stSlave.ucerro |= 0x02;
				ucerrortimes = 0;
			}
			break;
			
		default:
			break;
	}
	ucerrortimes++;	
}


static void ModbusInit(void)
{
	stHost.ucdevicenum = 1;
	ModbusDispath(ModFrameReceive);
}


static void ModbusPoll()
{
	(*ModbusRun())();
}


void ModbusSlaveInit()
{
	ModbusInit();
	
	Device.Usart3.Register(Receive);
	Device.Systick.Register(100,ModbusTimeExpire);
	Device.Systick.Register(100,ModbusPoll);
}
