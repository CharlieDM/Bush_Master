#ifndef __MODBUS_SLAVE_H
#define __MODBUS_SLAVE_H

/* ----------------------- Defines ------------------------------------------*/
#define MOD_ADDRESS_SLAVE         		( 0xC9 )    
#define MOD_SLAVE_ADDRESS_MIN          	( 1 )    
#define MOD_SLAVE_ADDRESS_MAX          	( 247 )  
#define MOD_SLAVE_SIZE_MAX				( 250 )
#define MOD_SLAVE_SIZE_MIN             	( 8 )

/* ----------------------- Typedefs ------------------------------------------*/
typedef struct _Slave
{
	uint8  ucmodstate;
	uint8  ucmodcmd;
	uint8  uctimeout;
	uint16 ustime;
	uint8  ucrevlen;
	uint8  *ucrevbuf;
	uint8  ucsendlen;
	uint8  ucSendBuf;
	uint8  ucerror;
}SlaveStruct;

extern SlaveStruct stSlave;

#endif
