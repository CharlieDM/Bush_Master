
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
static uint8 ucSendBuf[MOD_SLAVE_SIZE_MAX] = {0};

static uint16 ucTimes = 0;
static pFunc pfStateFunc = NULL;

SlaveStruct stSlave =
{
	MOD_FRAME,
	0,
	0,
	10,
	0,
	ucFrameBuf,
	0,
	ucSendBuf,
	MOD_EX_NONE
}

static void Receive(uint8 data)
{
	Queue_Push(&sg_tQueue,data);
}

static void ModSalveTimeExpire()
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

static void ModSlaveDispath(pFunc func)
{
	ucTimes = 0;
	stSlave.uctimeout = 0;
	pfStateFunc = func;
}


static pFunc ModSlaveRun(void)
{
	return pfStateFunc;
}


static void ModSlaveReceive()
{
	uint8 ucdata;
	uint8 ucpos = 0;
	uint8 uclen = 0;

	if(ucTimes == 0)
	{
		ucTimes = stSlave.ustime;
	}

	if(stSlave.uctimeout)
	{
		ModSlaveDispath(ModbusError);
	}
	
	stSlave.ucmodstate = MOD_FRAME;
	if(sg_tQueue.u8Cnt < MOD_SLAVE_SIZE_MIN) return;
	
	while( sg_tQueue.u8Cnt > 0 )
	{
		Queue_Pop(&sg_tQueue1,&ucdata);		
		if(ucdata != MOD_ADDRESS_SLAVE && ucpos == 0 ) continue; 	
	
		if(ucpos < MOD_SLAVE_SIZE_MAX)
		{
			ucFrameBuf[ucpos++] = ucdata;
		}

		if( ucpos == 2 )
		{
			switch(ucdata)
			{
				case MOD_FUNC_READ_MULTIPLE_REGISTER:
					uclen = 8;
					break;
				
				case MOD_FUNC_WRITE_SINGLE_REGISTER:				
					uclen = 10;
					break;
				
				case MOD_FUNC_WRITE_MULTIPLE_REGISTERS:
					Queue_Pop(&sg_tQueue1,&ucdata);	
					ucFrameBuf[ucpos++] = ucdata;
					Queue_Pop(&sg_tQueue1,&ucdata);	
					ucFrameBuf[ucpos++] = ucdata;
					Queue_Pop(&sg_tQueue1,&ucdata);	
					ucFrameBuf[ucpos++] = ucdata;
					Queue_Pop(&sg_tQueue1,&ucdata);	
					ucFrameBuf[ucpos++] = ucdata;
					uclen = ucFrameBuf[4]*256+ucFrameBuf[5];
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
		stSlave.ucrevlen= uclen;
		ModSlaveDispath(ModSlaveExec);
	}
	else
	{
		ModSlaveDispath(ModSlaveError);
	}	
}

static void ModSlaveExec()
{
	uint8 ucFunctionCode = 0;
	eMODException eException;	

	if(ucTimes == 0)
	{
		ucTimes = stSlave.ustime;
	}

	if(stHost.uctimeout)
	{
		ModSlaveDispath(ModSlaveError);
	}

	stSlave.ucmodstate = MOD_EXEC;
	ucFunctionCode = ucFrameBuf[MOD_FUNCTION_CODE];
	for( i=0; i<MOD_FUNC_HANDLERS_MAX;i++ )
	{
		if( xFuncHandler[i].ucFunctionCode == ucFunctionCode )
        {
            eException = xFuncHandler[i].pxHandler( stSlave.ucrevbuf, stSlave.ucrevlen );
            break;
        }
	}
	
	if(eException)
	{
		ModSlaveDispath(ModSlaveError);
	}
	else
	{
		ModSlaveDispath(ModSlaveReceive);
	}
}

static void ModSlaveError()
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


static void SlaveInit(void)
{
	stHost.ucdevicenum = 1;
	ModSlaveDispath(ModSlaveReceive);
}

static void ModSlavePoll()
{
	(*ModSlaveRun())();
}


void ModSlaveInit()
{
	SlaveInit();
	
	Device.Usart3.Register(Receive);
	Device.Systick.Register(100,ModSalveTimeExpire);
	Device.Systick.Register(100,ModSlavePoll);
}
