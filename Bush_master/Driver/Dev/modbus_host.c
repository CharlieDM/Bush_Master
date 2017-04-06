
#include "modproto.h"
#include "modbus_host.h"
#include "modfunc_host.h"
#include "core_hal.h"
#include "hal_systick.h"
#include "queue.h"
#include "crc.h"


static xMODFuncHandler xFuncHandler[] =
{
	{MOD_FUNC_READ_MULTIPLE_REGISTER,eModHostReadRegister},
	{MOD_FUNC_WRITE_SINGLE_REGISTER,eModHostWriteSingleRegister},
	{MOD_FUNC_WRITE_MULTIPLE_REGISTERS,eModHostWriteMultiRegister}
};


static xModCmd xCmdHanler[] =
{
	{0x07,{0x01,0x03,0x04,0x00,0x00,0x11,0x22}},
	{0x07,{0x02,0x03,0x04,0x00,0x00,0x11,0x22}},
};

static uint8 sg_au8Queue[MOD_HOST_SIZE_MAX] = {0};
static T_QUEUE_INFO sg_tQueue={sg_au8Queue,0,0,0,MOD_HOST_SIZE_MAX};
static uint8 ucFrameBuf[MOD_HOST_SIZE_MAX] = {0};

static uint16 ucTimes = 0;
static pFunc pfStateFunc = NULL;
static HostStruct stHost =
{
	MOD_INIT,								//modbus state
	MOD_FUNC_READ_MULTIPLE_REGISTER,		//cmd type
	10,										//expire  time
	0,										//time out state
	0,										//read cmd pos
	0,										//set cmd pos
	0										//device numbers
	0,										//frame len
	ucFrameBuf,								//frame buf
	0,
};

static void Receive(uint8 data)
{
	Queue_Push(&sg_tQueue,data);
}

static void ModHostTimeExpire()
{
	if(ucTimes) 
	{
		ucTimes--;	
		if(ucTimes == 0)
		{
			stHost.uctimeout = 1;
		}
	}
}

static void ModHostDispath(pFunc func)
{
	ucTimes = 0;
	stHost.uctimeout = 0;
	pfStateFunc = func;
}


static pFunc ModHostRun(void)
{
	return pfStateFunc;
}

static void ModHostSendCmd(void)
{
	uint8 ucRCmd = stHost.ucrcmdpos;
	if(stHost.ucscmdpos > 0)
	{
		//Device.Usart3.Send(xCmdHanler[ucCmdPos].pucBuf,xCmdHanler[ucCmdPos].ucLen);
	}
	else
	{
		Device.Usart3.Send(xCmdHanler[ucRCmd].pucBuf,xCmdHanler[ucRCmd].ucLen);
		stHost.ucmodcmd = MOD_FUNC_READ_MULTIPLE_REGISTER;
	}
	
	stHost.ucmodstate = MOD_SEND;
	ModHostDispath(ModHostReceive);
}

static void ModHostReceive(void)
{
	uint8 ucdata;
	uint8 ucpos = 0;
	uint8 uclen = 0;

	if(ucTimes == 0)
	{
		ucTimes = stHost.ustime;
	}

	if(stHost.uctimeout)
	{
		ModHostDispath(ModHostError);
	}
	
	stHost.ucmodstate = MOD_FRAME;
	if(sg_tQueue.u8Cnt < MOD_HOST_SIZE_MIN) return;
	
	while( sg_tQueue.u8Cnt > 0 )
	{
		Queue_Pop(&sg_tQueue1,&ucdata);		
		if(data != MOD_ADDRESS_HOST && ucpos == 0 ) continue; 	
	
		if(ucpos < MOD_HOST_SIZE_MAX)
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

	if(uclen >= MOD_HOST_SIZE_MIN && usMBCRC16(ucFrameBuf,uclen) == 0 )
	{
		stHost.ucframelen = uclen;
		ModHostDispath(ModHostExec);
	}
	else
	{
		ModHostDispath(ModHostError);
	}
	
}

static void ModHostExec()
{
	uint8 ucFunctionCode = 0;
	eMODException eException;	

	if(ucTimes == 0)
	{
		ucTimes = stHost.ustime;
	}

	if(stHost.uctimeout)
	{
		ModHostDispath(ModHostError);
	}

	stHost.ucmodstate = MOD_EXEC;
	ucFunctionCode = ucFrameBuf[MOD_FUNCTION_CODE];
	for( i=0; i<MOD_FUNC_HANDLERS_MAX;i++ )
	{
		if( xFuncHandler[i].ucFunctionCode == ucFunctionCode )
        {
            eException = xFuncHandler[i].pxHandler( stHost.ucbuf, stHost.ucframelen );
            break;
        }
	}
	if(!eException)
	{
		if(stHost.ucscmdpos > 0)
		{
			stHost.ucscmdpos = 0;
		}
		else
		{
			stHost.ucrcmdpos++;
		}
		ModHostDispath(ModbusSendCmd);
	}
	else
	{
		ModHostDispath(ModHostError);
	}
}

static void ModHostError()
{
	static uint8 ucerrortimes = 0;
	switch( stHost.ucmodcmd )
	{
		case MOD_FRAME:
			if(ucerrortimes >= 5)
			{
				stHost.ucerro |= 0x01;
				ucerrortimes = 0;
			}
			break;

		case MOD_EXEC:
			if(ucerrortimes >= 5)
			{
				stHost.ucerro |= 0x02;
				ucerrortimes = 0;
			}
			break;
			
		default:
			break;
	}
	ucerrortimes++;
}

static void HostInit(void)
{
	stHost.ucdevicenum = 1;
	ModHostDispath(ModHostSendCmd);
}

static void ModHostPoll()
{
	(*ModHostRun())();	
}

void ModHostInit()
{
	HostInit();
	
	Device.Usart3.Register(Receive);
	Device.Systick.Register(100,ModHostTimeExpire);
	Device.Systick.Register(100,ModHostPoll);
}
