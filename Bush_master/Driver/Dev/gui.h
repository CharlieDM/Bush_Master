#ifndef __GUI_H
#define __GUI_H

/* ----------------------- Defines ------------------------------------------*/
#define GUI_ADDRESS					( 0xF0 )    
#define GUI_SIZE_MAX			 	( 100 )
#define GUI_SIZE_MIN             	( 8 )

#define GUI_READ_MULTIPLE_REGISTER	       		( 0x03 )
#define GUI_WRITE_SINGLE_REGISTER             	( 0x06 )
#define GUI_WRITE_MULTIPLE_REGISTERS          	( 0x10 )

/* ----------------------- Typedefs ------------------------------------------*/
typedef struct _Gui
{
	uint8  ucstate;
	uint8  uctimeout;
	uint16 ustime;
	uint8  ucrevlen;
	uint8  *pucrevbuf;
	uint8  ucsendlen;
	uint8  *pucsendBuf;
	uint8  ucerror;
}GuiStruct; 

extern GuiStruct stGui;

#endif
