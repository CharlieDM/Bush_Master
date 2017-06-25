
#include "core_hal.h"
#include "modproto.h"
#include "modbus_slave.h"
#include "modfunc_slave.h"
#include "appdata.h"
#include "bittool.h"

static struct 
{
	void *pvAerkate;
}SlaveData;

/******************************************************************************
* Name       : void SlaveFuncInit(void)
* Function   : Init SlaveFuncInit and the data relation
******************************************************************************/
void SlaveFuncInit(void)
{
	SlaveData.pvAerkate = &Data.stAerkate;
}

/******************************************************************************
* Name       : eMODException eModSlaveReadRegister( uint8 * pucFrame, uint16 usLength )
* Function   : CMD:0x06
******************************************************************************/

eMODException  eModSlaveWriteSingleRegister( uint8 * pucFrame, uint16 usLength )
{
	uint16 usaddr = 0;
	if(usLength < MOD_SLAVE_SIZE_MIN) return MOD_EX_ILLEGAL_DATA_ADDRESS;
	usaddr = pucFrame[2]*256+pucFrame[3];
	U8CpyToU16(((uint16*)SlaveData.pvAerkate)+usaddr,&pucFrame[4],2);

	return MOD_EX_NONE;
}

/******************************************************************************
* Name       : eMODException eModSlaveWriteSingleRegister( uint8 * pucFrame, uint16 usLength )
* Function   : CMD:0x03
******************************************************************************/
eMODException eModSlaveReadRegister( uint8 * pucFrame, uint16 usLength )
{
	uint8 usaddr = 0,ulen = 0;
	if(usLength < MOD_SLAVE_SIZE_MIN) return MOD_EX_ILLEGAL_DATA_ADDRESS;
	usaddr = pucFrame[2]*256+pucFrame[3];
	ulen = pucFrame[4]*256+pucFrame[5];
	U16CpyToU8(&stSlave.ucSendBuf[3],((uint16*)SlaveData.pvAerkate)+usaddr,ulen);

	stSlave.ucSendBuf[0] = pucFrame[0];  
	stSlave.ucSendBuf[1] = 0x03;
	stSlave.ucSendBuf[2] = ulen*2;

	stSlave.ucsendlen = 5+ulen*2;
	return MOD_EX_NONE;
}

/******************************************************************************
* Name       : eMODException eModSlaveWriteMultiRegister( uint8 * pucFrame, uint16 usLength )
* Function   : CMD:0x10 
******************************************************************************/
eMODException eModSlaveWriteMultiRegister( uint8 * pucFrame, uint16 usLength )
{

	return MOD_EX_NONE;

}

