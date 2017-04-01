#ifndef __MODBUS_HOST_H
#define __MODBUS_HOST_H

/* ----------------------- Defines ------------------------------------------*/
#define MOD_ADDRESS_HOST        		( 0xF0 )    
#define MOD_HOST_ADDRESS_MIN          	( 1 )    
#define MOD_HOST_ADDRESS_MAX          	( 247 )  
#define MOD_HOST_SIZE_MAX			 	( 250 )
#define MOD_HOST_SIZE_MIN             	( 8 )

#define MOD_CMD_NORMAL					( 0x01 )
#define MOD_CMD_SPEC					( 0x02 )

/* ----------------------- Typedefs ------------------------------------------*/
typedef struct _Host
{
	uint8  ucmodstate;
	uint8  ucmodcmd;
	uint8  uctimeout;
	uint16 ustime;
	uint8  ucrcmdpos;
	uint8  ucscmdpos;
	uint8  ucdevicenum;
	uint8  ucframelen;
	uint8  *ucbuf;
	uint8  ucerror;
}HostStruct;



#endif
