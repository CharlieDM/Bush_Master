#ifndef __MODFUNC_SLAVE_H
#define __MODFUNC_SLAVE_H

#include "type_def.h"
#include "modproto.h"

/* ----------------------- Define Funcs ------------------------------------------*/
eMODException eModSlaveReadRegister( uint8 * pucFrame, uint16 pusLength );
eMODException eModSlaveWriteSingleRegister( uint8 * pucFrame, uint16 pusLength );
eMODException eModSlaveWriteMultiRegister( uint8 * pucFrame, uint16 pusLength );

#endif
