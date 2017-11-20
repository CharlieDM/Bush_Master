#ifndef __GUI_H
#define __GUI_H

#include "type_def.h"

/* ----------------------- Defines ------------------------------------------*/
#define GUI_ADDRESS					( 0x01 )    
#define GUI_SIZE_MAX			 	( 250 )
#define GUI_SIZE_MIN             	( 8 )
#define GUI_FUNC_HANDLERS_MAX		( 3 )

#define GUI_READ_MULTIPLE_REGISTER	       		( 0x04 )
#define GUI_WRITE_SINGLE_REGISTER             	( 0x06 )
#define GUI_WRITE_MULTIPLE_REGISTERS          	( 0x10 )

/* ----------------------- Typedefs ------------------------------------------*/
typedef enum
{
    GUI_INIT 	= 0x00,
    GUI_FRAME 	= 0x01,
    GUI_EXEC 	= 0x02,
    GUI_ERROR 	= 0x03,
} eGUIState;

typedef struct _Gui
{
	uint8  ucid;
	uint8  ucstate;
	uint8  uctimeout;
	uint16 ustime;
	uint8  ucrevlen;
	uint8  *pucrevbuf;
	uint8  ucsendlen;
	uint8  *pucsendBuf;
	uint8  ucerror;
}GuiStruct; 

/* ----------------------- Globel Var ------------------------------------------*/
extern GuiStruct stGui;
void GuiInit(void);
#endif
