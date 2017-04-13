#ifndef __GUI_FUNC_H
#define __GUI_FUNC_H

#include "type_def.h"


/* ----------------------- Macro definitions ---------------------------------*/

#define GUI_SENSOR_ONE			( 0x02BC)
#define GUI_SENSOR_TWO			( 0x02BC)
#define GUI_SENSOR_THREE		( 0x02BC)
#define GUI_SENSOR_FOUR			( 0x02BC)
#define GUI_SENSOR_FIVE			( 0x02BC)

/* ----------------------- Type definitions ---------------------------------*/
typedef enum
{
    GUI_EX_NULL = 0x00,
    GUI_EX_ILLEGAL_FUNCTION = 0x01,
    GUI_EX_ILLEGAL_DATA_ADDRESS = 0x02,
    GUI_EX_ILLEGAL_DATA_VALUE = 0x03,
} eGUIException;

/* ----------------------- Functions ---------------------------------*/
eGUIException eGuiReadRegister( uint8 * pucFrame, uint16 usLength );
eGUIException eGUiWriteSingleRegister( uint8 * pucFrame, uint16 usLength );
eGUIException eGuiWriteMultiRegister( uint8 * pucFrame, uint16 usLength );

#endif
