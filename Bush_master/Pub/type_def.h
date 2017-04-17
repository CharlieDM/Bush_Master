
#ifndef _TYPE_DEF_H_
#define _TYPE_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "stm32f10x.h"

/* Definition for tpyes */
typedef unsigned char         uint8;       /* Type definition for 8-bit unsigned char       */
typedef unsigned short int    uint16;      /* Type definition for 16-bit unsigned short int */
typedef unsigned int          uint32;      /* Type definition for 32-bit unsigned int       */

typedef signed char           int8;        /* Type definition for 8-bit signed char         */
typedef signed short int      int16;       /* Type definition for 16-bit signed short int   */
typedef signed int            int32;       /* Type definition for 32-bit signed int         */

typedef uint8 (*PF_TASK_PROCESS)(uint8 u8Evt, void *pPara);

#define NULL              		(0)         /* Value for NULL pointer */
#define SW_OK             		(0)         /* Return: Successful */
#define SW_ERR            		(0xFF)      /* Return: Failed     */
	
#define TRUE					(1)
#define FALSE					(0)

#define Enter_Critical()		//__disable_irq()
#define Leave_Critical()		//__denable_irq()

#ifdef __cplusplus
}
#endif

#endif /* _TYPE_DEF_H_ */

/* End of file */

