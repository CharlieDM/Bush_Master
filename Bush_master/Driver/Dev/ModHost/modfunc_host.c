
#include "core_hal.h"
#include "modproto.h"
#include "modfunc_host.h"
#include "modbus_host.h"
#include "bittool.h"
#include "appdata.h"

static struct 
{
	void *pvSensor;
	void *pvControl;
}HostData;

/******************************************************************************
* Name       : void HostFuncInit(void)
* Function   : Init Hostfunction and the data relation
******************************************************************************/
void HostFuncInit(void)
{
	HostData.pvSensor = Data.stSensor;
}

/******************************************************************************
* Name       : eMODException eModHostReadRegister( uint8 * pucFrame, uint16 pusLength )
* Function   : Deal CMD:0x03
******************************************************************************/
eMODException eModHostReadRegister( uint8 * pucFrame, uint16 usLength )
{
	uint8  ucid = 0;
	uint16 usaddr = stHost.usaddr;
	uint16 uslen = 0;
	/* Check data validation */
	if(usLength < MOD_HOST_SIZE_MIN) return MOD_EX_ILLEGAL_DATA_ADDRESS;
	if(pucFrame[2] == MOD_NACK_HEAD1 && pucFrame[3] == MOD_NACK_HEAD2) return MOD_EX_ILLEGAL_DATA_ADDRESS;

	/* Deal the data */
	ucid = pucFrame[0];
	uslen = pucFrame[2];
	
	if( ucid < 100)
	{
		U8CpyToU16(((int16 *)HostData.pvSensor)+ucid*25+usaddr,&pucFrame[3],uslen);
	}
	else if(ucid > 100)
	{
		
	}
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

