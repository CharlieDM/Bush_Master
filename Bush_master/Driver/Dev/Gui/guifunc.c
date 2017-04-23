
#include "core_hal.h"
#include "Gui.h"
#include "Guifunc.h"
#include "appdata.h"
#include "bittool.h"

static struct 
{
	void *pvSensor;
	void *pvControl;
}GuiData;

/******************************************************************************
* Name       : void GuiFuncInit(void)
* Function   : Init GuiFuncInit and the data relation
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
	if(usLength < GUI_SIZE_MIN) return GUI_EX_ILLEGAL_DATA_ADDRESS;
	
	/* Deal the data */
	usaddr = pucFrame[2]*256 + pucFrame[3];
	uslen  = pucFrame[4]*256 + pucFrame[5];

	if(uslen > 25) return GUI_EX_ILLEGAL_DATA_VALUE;
	
	switch(usaddr)
	{
		case GUI_SENSOR_ONE:
		case GUI_SENSOR_TWO:
		case GUI_SENSOR_THREE:
		case GUI_SENSOR_FOUR:
		case GUI_SENSOR_FIVE:
			stGui.pucsendBuf[0] = stGui.ucid;
			stGui.pucsendBuf[1] = 0x04;
			stGui.pucsendBuf[2] = uslen*2;
			U16CpyToU8(&stGui.pucsendBuf[3],((uint16 *)GuiData.pvSensor)+(usaddr-GUI_SENSOR_ONE)/25,uslen);
			stGui.ucsendlen = 5 + uslen*2;
			break;

		default:
			break;
	}
	
	return GUI_EX_NULL;
}

/******************************************************************************
* Name       : eGUIException eGuiWriteSingleRegister( uint8 * pucFrame, uint16 usLength )
* Function   : Deal CMD:0x06
******************************************************************************/
eGUIException eGuiWriteSingleRegister( uint8 * pucFrame, uint16 usLength )
{


	return GUI_EX_NULL;
}

/******************************************************************************
* Name       : eGUIException eGuiWriteMultiRegister( uint8 * pucFrame, uint16 usLength )
* Function   : Deal CMD:0x10
******************************************************************************/
eGUIException eGuiWriteMultiRegister( uint8 * pucFrame, uint16 usLength )
{

	return GUI_EX_NULL;

}

