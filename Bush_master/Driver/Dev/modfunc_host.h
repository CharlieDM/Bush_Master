#ifndef __MODFUNC_HOST_H
#define __MODFUNC_HOST_H


eMODException eModHostReadRegister( uint8 * pucFrame, uint16 * pusLength );
eMODException eModHostWriteSingleRegister( uint8 * pucFrame, uint16 * pusLength );
eMODException eModHostWriteMultiRegister( uint8 * pucFrame, uint16 * pusLength );





#endif
