
#include "modproto.h"
#include "modbus_slave.h"
#include "modfunc_slave.h"
#include "core_hal.h"
#include "hal_systick.h"
#include "queue.h"
#include "crc.h"
#include "appdata.h"
/* ----------------------- Static Func ------------------------------------------*/
static void ModSlaveError(void);
static void ModSlaveReceive(void);
static void ModSlaveExec(void);

/* ----------------------- Static Var ------------------------------------------*/
static xMODFuncHandler xFuncHandler[] =
{
	{MOD_FUNC_READ_MULTIPLE_REGISTER,eModSlaveReadRegister},
	{MOD_FUNC_WRITE_SINGLE_REGISTER,eModSlaveWriteSingleRegister},
	{MOD_FUNC_WRITE_MULTIPLE_REGISTERS,eModSlaveWriteMultiRegister}
};
static uint8 sg_au8Queue[MOD_SLAVE_SIZE_MAX] = {0};
static T_QUEUE_INFO sg_tQueue={sg_au8Queue,0,0,0,MOD_SLAVE_SIZE_MAX};
static uint8 ucRevBuf[MOD_SLAVE_SIZE_MAX] = {0};
static uint8 ucSendBuf[MOD_SLAVE_SIZE_MAX] = {0};
static uint16 ucTimes = 0;
static pFunc pfStateFunc = NULL;
SlaveStruct stSlave =
{
	MOD_FRAME,
	0,
	0,
	5,
	0,
	ucRevBuf,
	0,
	ucSendBuf,
	MOD_EX_NONE,
};

/******************************************************************************
* Name       : static void Receive(uint8 data)
* Function   : ModbusSlave receive a data
******************************************************************************/
static void Receive(uint8 data)
{
	Queue_Push(&sg_tQueue,data);
}

/******************************************************************************
* Name       : static void ModSalveTimeExpire(void)
* Function   : ModbusSlave State Expire time
******************************************************************************/
static void ModSalveTimeExpire(void)
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

/******************************************************************************
* Name       : static void ModSlaveReceive(void)
* Function   : ModbusSlave receive data deal and parser a Frame data
******************************************************************************/
static void ModSlaveReceive(void)
{
	uint8 ucdata;
	uint8 ucpos = 0;
	uint8 uclen = 0;
	
	stSlave.ucmodstate = MOD_FRAME;
	if(sg_tQueue.u8Cnt < MOD_SLAVE_SIZE_MIN) return;
	while( sg_tQueue.u8Cnt > 0 )
	{
		Queue_Pop(&sg_tQueue,&ucdata);		
		if(ucdata != Data.stAerkate.usId && ucpos == 0 ) continue; 	
	
		if(ucpos < MOD_SLAVE_SIZE_MAX)
		{
			ucRevBuf[ucpos++] = ucdata;
		}

		if( ucpos == 2 )
		{
			switch(ucdata)
			{
				case MOD_FUNC_READ_MULTIPLE_REGISTER:
					uclen = 8;
					break;
				
				case MOD_FUNC_WRITE_SINGLE_REGISTER:				
					uclen = 8;
					break;
				
				case MOD_FUNC_WRITE_MULTIPLE_REGISTERS:
					Queue_Pop(&sg_tQueue,&ucdata);	
					ucRevBuf[ucpos++] = ucdata;
					Queue_Pop(&sg_tQueue,&ucdata);	
					ucRevBuf[ucpos++] = ucdata;
					Queue_Pop(&sg_tQueue,&ucdata);	
					ucRevBuf[ucpos++] = ucdata;
					Queue_Pop(&sg_tQueue,&ucdata);	
					ucRevBuf[ucpos++] = ucdata;
					uclen = ucRevBuf[4]*256+ucRevBuf[5];
					break;
				
				default:	
					ucpos = 0;
					break;
			}
		}
		
		if( uclen > 0 && ucpos > 0 && ucpos >= uclen ) break;
	}

	if(uclen >= MOD_SLAVE_SIZE_MIN && usMBCRC16(ucRevBuf,uclen) == 0 )
	{
		stSlave.ucrevlen= uclen;
		ModSlaveDispath(ModSlaveExec);
	}
	else
	{
		ModSlaveDispath(ModSlaveError);
	}	
}

/******************************************************************************
* Name       : static void ModSlaveExec(void)
* Function   : ModbusSlave a Frame data deal
******************************************************************************/
static void ModSlaveExec(void)
{
	uint8 i =0;
	uint8 ucFunctionCode = 0;
	eMODException eException;	

	if(ucTimes == 0)
	{
		ucTimes = stSlave.ustime;
	}

	if(stSlave.uctimeout)
	{
		ModSlaveDispath(ModSlaveError);
	}

	stSlave.ucmodstate = MOD_EXEC;
	ucFunctionCode = ucRevBuf[MOD_FUNCTION_CODE];
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

/******************************************************************************
* Name       : static void ModSlaveError(void)
* Function   : ModbusSlave Error Deal
******************************************************************************/
static void ModSlaveError(void)
{
	static uint8 ucerrortimes = 0;
	switch( stSlave.ucmodcmd )
	{
		case MOD_FRAME:			
			if(ucerrortimes >= 5)
			{
				stSlave.ucerror |= 0x01;
				ucerrortimes = 0;
			}
			break;

		case MOD_EXEC:			
			if(ucerrortimes >= 5)
			{
				stSlave.ucerror |= 0x02;
				ucerrortimes = 0;
			}
			
			/* Fill the send data*/
			stSlave.ucSendBuf[0] = MOD_ADDRESS_SLAVE;
			stSlave.ucSendBuf[1] = 0x03;
			stSlave.ucSendBuf[2] = 0xAA;
			stSlave.ucSendBuf[3] = 0x55;
			break;
			
		default:
			break;
	}
	
	/* Change the State */
	ModSlaveDispath(ModSlaveReceive);
	ucerrortimes++;	
}

/******************************************************************************
* Name       : static void SlaveSend(void)
* Function   : ModbusSlave Send Data
******************************************************************************/
static void SlaveSend(void)
{
	uint16 usCRC = 0;
    if(stSlave.ucsendlen > 0)
    {
    	usCRC = usMBCRC16(stSlave.ucSendBuf,stSlave.ucsendlen-2);
        stSlave.ucSendBuf[stSlave.ucsendlen-2] = usCRC & 0xFF;
        stSlave.ucSendBuf[stSlave.ucsendlen-1] = usCRC>>8;
		Device.Usart5.Send(stSlave.ucSendBuf, stSlave.ucsendlen);
        stSlave.ucsendlen = 0;
    }	
}

/******************************************************************************
* Name       : static void SlaveInit(void)
* Function   : ModbusSlave Date&State Init
******************************************************************************/
static void SlaveInit(void)
{   
	SlaveFuncInit();
	ModSlaveDispath(ModSlaveReceive);
}

/******************************************************************************
* Name       : void ModSlavePoll(void)
* Function   : ModbusSlave Poll
******************************************************************************/
static void ModSlavePoll(void)
{
	(*ModSlaveRun())();
	SlaveSend();
}

/******************************************************************************
* Name       : void ModSlaveInit(void)
* Function   : ModbusSlave Init 
******************************************************************************/
void ModSlaveInit(void)
{
	SlaveInit();	
	Device.Usart5.Register(Receive);
	Device.Systick.Register(100,ModSalveTimeExpire);
	Device.Systick.Register(10,ModSlavePoll);
}

