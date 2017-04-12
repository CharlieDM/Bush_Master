
#include "core_hal.h"
#include "Guifunc.h"

static struct 
{
	void *pvSensor;
	void *pvControl;
}GuiData;

/******************************************************************************
* Name       : void HostFuncInit(void)
* Function   : Init Hostfunction and the data relation
******************************************************************************/
void GuiFuncInit(void)
{
	GuiData.pvSensor = Data.stSensor;
}

/******************************************************************************
* Name       : eGUIException eGuiReadRegister( uint8 * pucFrame, uint16 usLength )
* Function   : Deal CMD:0x03
******************************************************************************/
eGUIException eGuiReadRegister( uint8 * pucFrame, uint16 usLength )
{
	uint16 usaddr = 0;
	uint16 uslen = 0;
	
	/* Check data validation */
	if(usLength < GUI_SIZE_MIN) return MOD_EX_ILLEGAL_DATA_ADDRESS;

	/* Deal the data */
	usaddr = pucFrame[2]*256 + pucFrame[3];
	uslen  = pucFrame[4]*256 + pucFrame[5];

	switch(usaddr)
	{
		case 0x0000;
			
			
			break;

		default:
			break;
	}
	
	return MOD_EX_NONE;
}

/******************************************************************************
* Name       : eGUIException eGuiWriteSingleRegister( uint8 * pucFrame, uint16 usLength )
* Function   : Deal CMD:0x06
******************************************************************************/
eGUIException eGuiWriteSingleRegister( uint8 * pucFrame, uint16 usLength )
{


	return MOD_EX_NONE;
}

/******************************************************************************
* Name       : eGUIException eGuiWriteMultiRegister( uint8 * pucFrame, uint16 usLength )
* Function   : Deal CMD:0x10
******************************************************************************/
eGUIException eGuiWriteMultiRegister( uint8 * pucFrame, uint16 usLength )
{

	return MOD_EX_NONE;

}

