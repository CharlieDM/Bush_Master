
#include "core_hal.h"
#include "modproto.h"
#include "modfunc_host.h"
#include "modbus_host.h"

static struct 
{
	void *Sensor;
	void *Control;
}HostDataStruct;



/******************************************************************************
* Name       : eMODException eModHostReadRegister( uint8 * pucFrame, uint16 pusLength )
* Function   : Deal CMD:0x03
******************************************************************************/

eMODException eModHostReadRegister( uint8 * pucFrame, uint16 usLength )
{
	uint8  ucid = 0;
	uint16 usaddr = 0;
	uint16 uslen = 0;
	if(usLength < MOD_HOST_SIZE_MIN) return MOD_EX_ILLEGAL_DATA_ADDRESS;

	ucid = pucFrame[0];
	usaddr = pucFrame[2]*256+pucFrame[3];
	uslen = pucFrame[4]*256+pucFrame[5];

	stHost.ucSendBuf[0] = pucFrame[0];
	stHost.ucSendBuf[1] = pucFrame[1];
	stHost.ucSendBuf[2] = uslen*2;

	stHost.ucsendlen = 3+uslen*2;
	return MOD_EX_NONE;
}

/******************************************************************************
* Name       : eModHostWriteSingleRegister( uint8 * pucFrame, uint16 pusLength )
* Function   : Deal CMD:0x06
******************************************************************************/
eMODException eModHostWriteSingleRegister( uint8 * pucFrame, uint16 usLength )
{


return MOD_EX_NONE;

}

/******************************************************************************
* Name       : eModHostWriteMultiRegister( uint8 * pucFrame, uint16 pusLength )
* Function   : Deal CMD:0x06
******************************************************************************/
eMODException eModHostWriteMultiRegister( uint8 * pucFrame, uint16 usLength )
{


return MOD_EX_NONE;

}

