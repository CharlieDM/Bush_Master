
#ifndef _QUEUE_H_
#define _QUEUE_H_

#ifdef __cplusplus
extern "C" {
#endif
	
#include "type_def.h"

/* THe last free block can be push */
#define QUEUE_LAST_FREE(p)        ((p)->pu8Addr + (p)->u8End)
/* The first used block can be pop */
#define QUEUE_FIRST_USED(p)       ((p)->pu8Addr + (p)->u8Begin)

/*******************************************************************************
* Structure  : T_QUEUE_INFO
* Description: Structure of queue information.
*******************************************************************************/
typedef struct _T_QUEUE_INFO
{
    uint8 *pu8Addr;                    /* Address of the queue                */
    uint8  u8Begin;                    /* The begin pointer--first used block */
    uint8  u8End;                      /* The end pointer--last free block    */
    uint8  u8Cnt;                      /* The count of used blocks            */
    uint8  u8Len;                      /* The length of each block            */
}T_QUEUE_INFO;

uint8 Queue_Create(T_QUEUE_INFO *ptQueueInfo, uint8 u8Len);
uint8 Queue_Delete(T_QUEUE_INFO *ptQueueInfo);
uint8 Queue_Reset(T_QUEUE_INFO *ptQueue);
uint8 Queue_Is_Free(T_QUEUE_INFO *ptQueue);
uint8 Queue_Is_Not_Empty(T_QUEUE_INFO *ptQueue);
uint8 Queue_Push(T_QUEUE_INFO *ptQueueInfo, uint8 pu8Data);
uint8 Queue_Pop(T_QUEUE_INFO* ptQueueInfo, uint8 *pu8Data);

#ifdef __cplusplus
}
#endif

#endif /* _QUEUE_H_ */

/* End of file */

