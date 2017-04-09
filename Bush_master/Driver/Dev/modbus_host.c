
#include "modproto.h"
#include "modbus_host.h"
#include "modfunc_host.h"
#include "core_hal.h"
#include "hal_systick.h"
#include "queue.h"
#include "crc.h"

/* ----------------------- Static Function ------------------------------------------*/
static void ModHostSendCmd(void);
static void ModHostReceive(void);
static void ModHostExec(void);
static void ModHostError(void);

/* ----------------------- Static Var ------------------------------------------*/
static xMODFuncHandler xFuncHandler[] =
{
	{MOD_FUNC_READ_MULTIPLE_REGISTER,eModHostReadRegister},
	{MOD_FUNC_WRITE_SINGLE_REGISTER,eModHostWriteSingleRegister},
	{MOD_FUNC_WRITE_MULTIPLE_REGISTERS,eModHostWriteMultiRegister}
};

static xModCmd xCmdHanler[] =
{
	{0x08,{0x01, 0x03, 0x00, 0x00, 0x00, 0x2C, 0x44, 0x17 }}
	//{0x07,{0x02,0x03,0x04,0x00,0x00,0x11,0x22}},
};

static uint8 sg_au8Queue[MOD_HOST_SIZE_MAX] = {0};
static T_QUEUE_INFO sg_tQueue={sg_au8Queue,0,0,0,MOD_HOST_SIZE_MAX};
static uint8 ucFrameBuf[MOD_HOST_SIZE_MAX] = {0};
static uint8 ucSnedeBuf[MOD_HOST_SIZE_MAX] = {0};

static uint16 ucTimes = 0;
static pFunc pfStateFunc = NULL;
HostStruct stHost =
{
	0x00,									//slave address
	MOD_INIT,								//modbus state
	MOD_FUNC_READ_MULTIPLE_REGISTER,		//cmd type
	10,										//expire  time
	0,										//time out state
	0,										//read cmd pos
	0,										//set cmd pos
	0,										//device numbers
	0,										//frame len
	ucFrameBuf,								//frame buf
	0,
	ucSnedeBuf,
	0,
};

/******************************************************************************
* Name       : void Receive(uint8 data)
* Function   : Receive the data
******************************************************************************/

static void Receive(uint8 data)
{
	Queue_Push(&sg_tQueue,data);
}

/******************************************************************************
* Name       : void ModHostTimeExpire(void)
* Function   : host Time expire
******************************************************************************/
static void ModHostTimeExpire(void)
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

/******************************************************************************
* Name       : void ModHostDispath(pFunc func)
* Function   : State Transfrom
******************************************************************************/
static void ModHostDispath(pFunc func)
{
	ucTimes = 0;
	stHost.uctimeout = 0;
	pfStateFunc = func;
}

/******************************************************************************
* Name       : void ModHostRun(void)
* Function   : return a function point
******************************************************************************/
static pFunc ModHostRun(void)
{
	return pfStateFunc;
}

/******************************************************************************
* Name       : void ModHostSendCmd(void)
* Function   : Send the CMD
******************************************************************************/
static void ModHostSendCmd(void)
{
	uint8 ucRCmd = stHost.ucrcmdpos;
	if(stHost.ucscmdpos > 0)
	{
		//Device.Usart3.Send(xCmdHanler[ucCmdPos].pucBuf,xCmdHanler[ucCmdPos].ucLen);
	}
	else
	{
		Device.Usart3.Send(xCmdHanler[ucRCmd].pcBuf,xCmdHanler[ucRCmd].ucLen);
		stHost.ucmodcmd = MOD_FUNC_READ_MULTIPLE_REGISTER;
		stHost.ucaddr = xCmdHanler[ucRCmd].pcBuf[0];
	}
	
	stHost.ucmodstate = MOD_SEND;
	ModHostDispath(ModHostReceive);
}

/******************************************************************************
* Name       : void ModHostReceive(void)
* Function   : Check a Frame data
******************************************************************************/
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
		Queue_Pop(&sg_tQueue,&ucdata);		
		if(ucdata != stHost.ucaddr && ucpos == 0 ) continue; 	
	
		if(ucpos < MOD_HOST_SIZE_MAX)
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
					Queue_Pop(&sg_tQueue,&ucdata);	
					ucFrameBuf[ucpos++] = ucdata;
					Queue_Pop(&sg_tQueue,&ucdata);	
					ucFrameBuf[ucpos++] = ucdata;
					Queue_Pop(&sg_tQueue,&ucdata);	
					ucFrameBuf[ucpos++] = ucdata;
					Queue_Pop(&sg_tQueue,&ucdata);	
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

/******************************************************************************
* Name       : void ModHostExec(void)
* Function   : Host Receive a frame and exec function
******************************************************************************/
static void ModHostExec(void)
{
	uint8 i = 0;
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
		ModHostDispath(ModHostSendCmd);
	}
	else
	{
		ModHostDispath(ModHostError);
	}
}

/******************************************************************************
* Name       : void ModHostError(void)
* Function   : Host Error Deal
******************************************************************************/
static void ModHostError(void)
{
	static uint8 ucerrortimes = 0;
	switch( stHost.ucmodcmd )
	{
		case MOD_FRAME:
			if(ucerrortimes >= 5)
			{
				stHost.ucerror |= 0x01;
				ucerrortimes = 0;
			}
			break;

		case MOD_EXEC:
			if(ucerrortimes >= 5)
			{
				stHost.ucerror |= 0x02;
				ucerrortimes = 0;
			}
			break;
			
		default:
			break;
	}
	ModHostDispath(ModHostSendCmd);
	ucerrortimes++;
}

/******************************************************************************
* Name       : void HostInit(void)
* Function   : Host Data&State Init
******************************************************************************/
static void HostInit(void)
{
	stHost.ucdevicenum = 1;
	ModHostDispath(ModHostSendCmd);
}

/******************************************************************************
* Name       : void ModHostPoll()
* Function   : Host Poll
******************************************************************************/
static void ModHostPoll(void)
{
	(*ModHostRun())();	
}

/******************************************************************************
* Name       : void ModHostInit()
* Function   : init ModHost
******************************************************************************/
void ModHostInit(void)
{
	HostInit();
	
	Device.Usart3.Register(Receive);
	Device.Systick.Register(100,ModHostTimeExpire);
	Device.Systick.Register(100,ModHostPoll);
}

