
#include "core_hal.h"
#include "gui.h"
#include "guifunc.h"
#include "hal_systick.h"
#include "queue.h"
#include "crc.h"
#include "modproto.h"

/* ----------------------- static Function ------------------------------------------*/
static void GuiTimeExpire(void);
static void GuiError(void);
static void GuiReceive(void);
static void GuiExec(void);

/* ----------------------- static var ------------------------------------------*/
static xGuiFuncHandler xFuncHandler[] =
{
	{GUI_READ_MULTIPLE_REGISTER,eGuiReadRegister},
	{GUI_WRITE_SINGLE_REGISTER,eGuiWriteSingleRegister},
	{GUI_WRITE_MULTIPLE_REGISTERS,eGuiWriteMultiRegister}
};
static uint8 ucQueue[GUI_SIZE_MAX] = {0};
static T_QUEUE_INFO sg_tQueue={ucQueue,0,0,0,GUI_SIZE_MAX};
static uint8 ucRevBuf[GUI_SIZE_MAX] = {0};
static uint8 ucSendBuf[GUI_SIZE_MAX] = {0};
static uint16 ucTimes = 0;
static pFunc pfStateFunc = NULL;
GuiStruct stGui =
{
	GUI_ADDRESS,
	GUI_INIT,
	0,
	100,
	0,
	ucRevBuf,
	0,
	ucSendBuf,
	0,
};

/******************************************************************************
* Name       : static void Receive(uint8 data)
* Function   : Recive data
******************************************************************************/

static void Receive(uint8 data)
{
	Queue_Push(&sg_tQueue,data);
}

/******************************************************************************
* Name       : static void GuiTimeExpire(void)
* Function   : Count the times
******************************************************************************/
static void GuiTimeExpire(void)
{
	if(ucTimes) 
	{
		ucTimes--;	
		if(ucTimes == 0)
		{
			stGui.uctimeout= 1;
		}
	}
}

/******************************************************************************
* Name       : static void GuiDispath(pFunc func)
* Function   : Dispath the GUI state
******************************************************************************/
static void GuiDispath(pFunc func)
{
	ucTimes = 0;
	stGui.uctimeout = 0;
	pfStateFunc = func;
}

/******************************************************************************
* Name       : static pFunc GuiRun(void)
* Function   : GUI Run Function
******************************************************************************/
static pFunc GuiRun(void)
{
	return pfStateFunc;
}

/******************************************************************************
* Name       : static void GuiReceive(void)
* Function   : GUI Receive data
******************************************************************************/
static void GuiReceive(void)
{
	uint8 ucdata;
	uint8 ucpos = 0;
	uint8 uclen = 0;
	
	stGui.ucstate = GUI_FRAME;
	if(stGui.ucrevlen < GUI_SIZE_MIN) return;
	
	while( sg_tQueue.u8Cnt > 0 )
	{
		Queue_Pop(&sg_tQueue,&ucdata);		
		if(ucdata != GUI_ADDRESS && ucpos == 0 ) continue; 	
	
		if(ucpos < GUI_SIZE_MAX)
		{
			ucRevBuf[ucpos++] = ucdata;
		}

		if( ucpos == 2 )
		{
			switch(ucdata)
			{
				case GUI_READ_MULTIPLE_REGISTER:
					uclen = 8;
					break;
				
				case GUI_WRITE_SINGLE_REGISTER:				
					uclen = 10;
					break;
				
				case GUI_WRITE_MULTIPLE_REGISTERS:
					uclen = ucRevBuf[4]*256+ucRevBuf[5];
					break;
				
				default:	
					ucpos = 0;
					break;
			}
		}
		
		if( uclen > 0 && ucpos > 0 && ucpos >= uclen ) break;
	}

	if(uclen >= GUI_SIZE_MIN && usMBCRC16(ucRevBuf,uclen) == 0 )
	{
		stGui.ucrevlen= uclen;
		GuiDispath(GuiExec);
	}
	else
	{
		GuiDispath(GuiError);
	}	
}

/******************************************************************************
* Name       : static void GuiExec(void)
* Function   : GUI Exec Function
******************************************************************************/
static void GuiExec(void)
{
	uint8 i =0;
	uint8 ucFunctionCode = 0;
	eGUIException eException;	

	if(ucTimes == 0)
	{
		ucTimes = stGui.ustime;
	}

	if(stGui.uctimeout)
	{
		GuiDispath(GuiError);
	}

	stGui.ucstate = GUI_EXEC;
	ucFunctionCode = ucRevBuf[MOD_FUNCTION_CODE];
	for( i=0; i<GUI_FUNC_HANDLERS_MAX;i++ )
	{
		if( xFuncHandler[i].ucFunctionCode == ucFunctionCode )
        {
            eException = xFuncHandler[i].pxHandler( stGui.pucrevbuf, stGui.ucrevlen );
            break;
        }
	}
	
	if(eException)
	{
		GuiDispath(GuiError); 
	}
	else
	{
		GuiDispath(GuiReceive); 
	}
}

/******************************************************************************
* Name       : static void GuiError(void)
* Function   : GUI Error Deal
******************************************************************************/
static void GuiError(void)
{
	static uint8 ucerrortimes = 0;
	switch( stGui.ucstate )
	{
		case MOD_FRAME:			
			if(ucerrortimes >= 5)
			{
				stGui.ucerror |= 0x01;
				ucerrortimes = 0;
			}
			break;

		case MOD_EXEC:			
			if(ucerrortimes >= 5)
			{
				stGui.ucerror |= 0x02;
				ucerrortimes = 0;
			}
			break;
			
		default:
			break;
	}
	
	/* Change the State*/
	GuiDispath(GuiReceive); 
	ucerrortimes++;	
}

/******************************************************************************
* Name       : static void GuiSend(void)
* Function   : GUI Send Data
******************************************************************************/
static void GuiSend(void)
{
	uint16 usCRC = 0;
    if(stGui.ucsendlen > 0)
    {	
		usCRC = usMBCRC16(stGui.pucsendBuf,stGui.ucsendlen-2);
        stGui.pucsendBuf[stGui.ucsendlen-2] = usCRC & 0xFF;
        stGui.pucsendBuf[stGui.ucsendlen-1] = usCRC>>8;
		Device.Usart3.Send(stGui.pucsendBuf, stGui.ucsendlen);
        stGui.ucsendlen = 0;
    }	
}

/******************************************************************************
* Name       : static void GuiPoll(void)
* Function   : GUI State Loop
******************************************************************************/
static void GuiPoll(void)
{
	(*GuiRun())();
	GuiSend();
}

/******************************************************************************
* Name       : void GUiInit(void)
* Function   : GUI Init 
******************************************************************************/
void GuiInit(void)
{
	GuiDispath(GuiReceive);
	
	Device.Usart3.Register(Receive);
	Device.Systick.Register(100,GuiTimeExpire);
	Device.Systick.Register(100,GuiPoll);
}

