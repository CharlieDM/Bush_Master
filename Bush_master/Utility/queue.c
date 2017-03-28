

#include "Queue.h"
#include "debug.h"
#include "stdlib.h"

/******************************************************************************
* Name       : uint8 Queue_Create(T_QUEUE_INFO *ptQueueInfo, uint8 u8Len, uint8 u8Cnt)
* Function   : Create a queue
******************************************************************************/
uint8 Queue_Create(T_QUEUE_INFO *ptQueueInfo, uint8 u8Len)
{
    /* If the buffer size is 0, return NULL */
    CHECK_IF_RET_ST((NULL == ptQueueInfo), "Invalid pointer!\n");
    CHECK_IF_RET_ST((0 == u8Len), "The length of buffer is invalid!!\n");

    ptQueueInfo->pu8Addr  = (uint8*)malloc(sizeof(uint8));   	   /* Create data space  */
    ptQueueInfo->u8Begin  = 0;                             /* Init begin pointer */
    ptQueueInfo->u8End    = 0;                             /* Init end pointer   */
    ptQueueInfo->u8Cnt    = 0;                             /* Init block count   */
    ptQueueInfo->u8Len    = u8Len;                         /* Set block length   */

    return SW_OK;
}

/******************************************************************************
* Name       : uint8 Queue_Delete(T_QUEUE_INFO *ptQueueInfo)
* Function   : Delete a queue
******************************************************************************/
uint8 Queue_Delete(T_QUEUE_INFO *ptQueueInfo)
{
    /* If the pointer of queue information is invalid */
    CHECK_IF_RET_ST((NULL == ptQueueInfo), "The length of buffer is invalid!!\n");

    free(ptQueueInfo->pu8Addr);

    return SW_OK;
}

/******************************************************************************
* Name       : uint8 Queue_Inc(T_QUEUE_INFO *ptQueue)
* Function   : Update the end pointer of used block.
******************************************************************************/
uint8 Queue_Reset(T_QUEUE_INFO *ptQueue)
{
    /* If the pointer of queue info is invalid */
    CHECK_IF_RET_ST((NULL == ptQueue), "Invalid pointer of queue information!!\n");

    ptQueue->u8Begin  = 0;                             /* Init begin pointer */
    ptQueue->u8End    = 0;                             /* Init end pointer   */
    ptQueue->u8Cnt    = 0;                             /* Init block count   */
	
    return SW_OK;
}

/******************************************************************************
* Name       : uint8 Queue_Is_Free(T_QUEUE_INFO *ptQueue)
* Function   : Check if the queue is free
******************************************************************************/
uint8 Queue_Is_Free(T_QUEUE_INFO *ptQueue)
{
    /* If the pointer of queue info is invalid */
    CHECK_IF_RET_ST((NULL == ptQueue), "Invalid pointer of queue information!!\n");

    /* If the used count is NOT less then the max count */
    CHECK_IF_RET_ST((ptQueue->u8Cnt >= ptQueue->u8Len), "The queue is full!!\n");

    return SW_OK;
}

/******************************************************************************
* Name       : uint8 Queue_Is_Not_Empty(T_QUEUE_INFO *ptQueue)
* Function   : Check if the queue is NOT empty
******************************************************************************/
uint8 Queue_Is_Not_Empty(T_QUEUE_INFO *ptQueue)
{
    /* If the pointer of queue info is invalid */
    CHECK_IF_RET_ST((NULL == ptQueue), "Invalid pointer of queue information!!\n");

    /* If the used count is NOT 0 */
    CHECK_IF_RET_ST((0 == ptQueue->u8Cnt), "The queue is empty!!\n");

    return SW_OK;
}

/******************************************************************************
* Name       : uint8 Queue_Push(T_QUEUE_INFO *ptQueueInfo, uint8 *pu8Data, uint8 u8Len)
* Function   : Write into the queue with the desired data
******************************************************************************/
uint8 Queue_Push(T_QUEUE_INFO *ptQueueInfo, uint8 pu8Data)
{
    /* If the pointer of queue information is invalid */
    CHECK_IF_RET_ST((NULL == ptQueueInfo)\
                     || (NULL == ptQueueInfo->pu8Addr)\
                     || (NULL == pu8Data),\
                     "Invalid pointer of queue information or data!!\n");
    
  
    /* If the used count is NOT less then the max count */
    CHECK_IF_RET_ST((ptQueueInfo->u8Cnt >= ptQueueInfo->u8Len), "The queue is full!!\n");

    *QUEUE_LAST_FREE(ptQueueInfo) = pu8Data;
    ptQueueInfo->u8End = (ptQueueInfo->u8End + 1) % ptQueueInfo->u8Len;  /* Update end pointer */
    ptQueueInfo->u8Cnt++;                                                   /* Update used count  */
   
    return SW_OK;
}

/******************************************************************************
* Name       : uint8 Queue_Pop(T_QUEUE_INFO *ptQueueInfo, uint8 *pu8Data, uint8 u8Len)
* Function   : Write into the queue with the desired data
******************************************************************************/
uint8 Queue_Pop(T_QUEUE_INFO* ptQueueInfo, uint8 *pu8Data)
{
    /* If the pointer of queue information is invalid */
    CHECK_IF_RET_ST((NULL == ptQueueInfo)\
                     || (NULL == ptQueueInfo->pu8Addr)
                     || (NULL == pu8Data),\
                     "Invalid pointer of queue information or data!!\n");
        
    /* If the used count is 0 */
    CHECK_IF_RET_ST((0 == ptQueueInfo->u8Cnt), "The queue is empty!!\n");

		*pu8Data = *QUEUE_FIRST_USED(ptQueueInfo);
    ptQueueInfo->u8Begin = (ptQueueInfo->u8Begin + 1) % ptQueueInfo->u8Len;  /* Update the begin pointer */
    ptQueueInfo->u8Cnt--;                                               /* Update the used count    */
    
    return SW_OK;   
}

/* end of file */

