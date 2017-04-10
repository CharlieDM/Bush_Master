#ifndef __MODFUNC_HOST_H
#define __MODFUNC_HOST_H

#include "type_def.h"
#include "modproto.h"

void HostFuncInit(void);
eMODException eModHostReadRegister( uint8 * pucFrame, uint16 usLength );
eMODException eModHostWriteSingleRegister( uint8 * pucFrame, uint16 usLength );
eMODException eModHostWriteMultiRegister( uint8 * pucFrame, uint16 usLength );

#endif
