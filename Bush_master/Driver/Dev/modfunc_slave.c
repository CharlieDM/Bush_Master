
#include "core_hal.h"
#include "modproto.h"
#include "modfunc_slave.h"

eMODException eModSlaveReadRegister( uint8 * pucFrame, uint16 usLength )
{
	uint16 usaddr = 0;
	uint16 uslen = 0;
	if(usLength < MOD_SLAVE_SIZE_MIN) return MOD_EX_ILLEGAL_DATA_ADDRESS;

	usaddr = pucFrame[2]*256+pucFrame[3];
	uslen = pucFrame[4]*256+pucFrame[5];

	stSlave.ucSendBuf[0] = pucFrame[0];
	stSlave.ucSendBuf[1] = pucFrame[1];
	stSlave.ucSendBuf[2] = uslen*2;

	stSlave.ucsendlen = 3+uslen*2;
	return MOD_EX_NONE;
}

eMODException eModSlaveWriteSingleRegister( uint8 * pucFrame, uint16 usLength )
{

	uint16 usaddr = 0;
	uint16 uslen = 0;
	if(usLength < MOD_SLAVE_SIZE_MIN) return MOD_EX_ILLEGAL_DATA_ADDRESS;

	usaddr = pucFrame[2]*256+pucFrame[3];
	uslen = 0x01;

	stSlave.ucSendBuf[0] = pucFrame[0];  
	stSlave.ucSendBuf[1] = pucFrame[1];
	stSlave.ucSendBuf[2] = pucFrame[2];
	stSlave.ucSendBuf[3] = pucFrame[3];
	stSlave.ucSendBuf[4] = 0x00;
	stSlave.ucSendBuf[5] = 0x01;

	stSlave.ucsendlen = 6;
	return MOD_EX_NONE;
}

eMODException eModSlaveWriteMultiRegister( uint8 * pucFrame, uint16 usLength )
{
	uint16 usaddr = 0;
	uint16 uslen = 0;
	if(usLength < MOD_SLAVE_SIZE_MIN) return MOD_EX_ILLEGAL_DATA_ADDRESS;

	usaddr = pucFrame[2]*256+pucFrame[3];
	uslen = pucFrame[4]*256+pucFrame[5];

	stSlave.ucSendBuf[0] = pucFrame[0];  
	stSlave.ucSendBuf[1] = pucFrame[1];
	stSlave.ucSendBuf[2] = pucFrame[2];
	stSlave.ucSendBuf[3] = pucFrame[3];
	stSlave.ucSendBuf[4] = pucFrame[4];
	stSlave.ucSendBuf[5] = pucFrame[5];

	stSlave.ucsendlen = 6;
	return MOD_EX_NONE;

}

