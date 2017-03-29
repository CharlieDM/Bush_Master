
#ifndef _MOD_PROTO_H
#define _MOD_PROTO_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "type_def.h"
	
/* ----------------------- Defines ------------------------------------------*/
#define MOD_FUNC_NONE                          		( 0x0 )
#define MOD_FUNC_READ_MULTIPLE_REGISTER	       		( 0x03 )
#define MOD_FUNC_WRITE_SINGLE_REGISTER             	( 0x06 )
#define MOD_FUNC_WRITE_MULTIPLE_REGISTERS          	( 0x10 )

#define MOD_FUNCTION_CODE		( 0x02 )
#define MOD_FUNC_HANDLERS_MAX	( 0x03 )

#define MOD_INVALID_DATA		( 0x00 )
#define MOD_OK					( 0x01 )
#define MOD_NOT_YET  			( 0x00 )



/* ----------------------- Type definitions ---------------------------------*/
typedef enum
{
    MOD_EX_NONE = 0x00,
    MOD_EX_ILLEGAL_FUNCTION = 0x01,
    MOD_EX_ILLEGAL_DATA_ADDRESS = 0x02,
    MOD_EX_ILLEGAL_DATA_VALUE = 0x03,
    MOD_EX_SLAVE_DEVICE_FAILURE = 0x04,
} eMODException;

typedef enum
{
    MOD_INIT = 0x00,
    MOD_FRAME = 0x01,
    MOD_EXEC = 0x02,
    MOD_ERROR = 0x03,
} eMODState;

typedef eMODException ( *pxMODFuncHandler ) ( uint8 * pucFrame, uint16 pusLength );

typedef struct
{
    uint8          	 ucFunctionCode;
    pxMODFuncHandler pxHandler;
} xMODFuncHandler;

typedef struct
{
    uint8  ucLen;
    uint8 *pucBuf;
} xModCmd;


#ifdef __cplusplus
}
#endif
#endif
