
#include "type_def.h"
#include "Core_os.h"
#include "Core_Event.h"
#include "debug.h"

static uint16 sg_u16EvtFirst = 0;  /* The fisrt available data position */
static uint16 sg_u16EvtCnt   = 0;  /* The count of availabe data        */
static EventStruct sg_atEvtQueue[MAX_QUEUE_NUM] = {0};

/******************************************************************************
* Name       : uint8 Event_Init(void)
* Function   : Init event queue
******************************************************************************/
uint8 Event_Init(void)
{   
	uint8 u8Idx;
	uint8 u8Len = MAX_QUEUE_NUM * sizeof(EventStruct);
    /* Check if the pointer is invalid or NOT */
    CHECK_IF_RET_ST(((uint8*)sg_atEvtQueue == NULL),"Memset invalid input\n");
    
    /* Loop for the desired length bytes to be set */
    for(u8Idx = 0; u8Idx < u8Len; u8Idx++)
    {
        (uint8*)sg_atEvtQueue[u8Idx] = 0;   /* Set with the desired value */
    }
    return SW_OK;
}

/******************************************************************************
* Name       : uint8 Event_Set(uint8 u8TaskID, uint8 u8Evt, uint8 u8Urg, void *pPara)
* Function   : This function is used to set event for tasks.
******************************************************************************/
uint8 Event_Set(uint8 u8TaskID, uint8 u8Evt, uint8 u8Urg, void *pPara) 
{  
	uint16 u16EvtLast;

    /* Check if the task ID is invalid or NOT */
    CHECK_IF_RET_ST((u8TaskID > MAX_TASK_NUM),"Task ID should be valid\n");

	/* If the current event count is equal to the max limit */
    CHECK_IF_RET_ST((MAX_QUEUE_NUM == sg_u16EvtCnt), "Event queue is full\n");
	
	/* If it is an urgent event */
	if(EVENT_URGENT == u8Urg)
	{
		if(0 == sg_u16EvtFirst) 	  /* If the first available data is in 0 position */
		{
			u16EvtLast = MAX_QUEUE_NUM - 1;  /* Calculate the last empty position */
		}
		else
		{
			u16EvtLast = sg_u16EvtFirst - 1;	 /* Calculate the last empty position */
		}
		sg_u16EvtFirst = u16EvtLast;  /* Update the first event position */
	}
	else/* If it is a normal event */
	{	/* Calculate the position for new event */
		u16EvtLast = (sg_u16EvtFirst + sg_u16EvtCnt) % MAX_QUEUE_NUM;	 
	}
	sg_atEvtQueue[u16EvtLast].pPara  = pPara;			  /* Fill the parameter 		  */
	sg_atEvtQueue[u16EvtLast].u8Task = u8TaskID;		  /* Fill the destination task ID */
	sg_atEvtQueue[u16EvtLast].u8Evt  = u8Evt;			  /* Fill the evnet 			  */
	sg_u16EvtCnt++; 									  /* Update event count 		  */

	return SW_OK;

}


/******************************************************************************
* Name       : T_EVENT* Event_Get(void)
* Function   : To get a event from quque
******************************************************************************/
EventStruct* Event_Get(void)
{   
    uint16 u16EvtFirst;
    /* If there is no event */
    CHECK_IF_RET_VAL((0 == sg_u16EvtCnt), NULL, "Event count is 0\n");
    CHECK_IF_RET_VAL((0 == sg_atEvtQueue[sg_u16EvtFirst].u8Task), NULL, "Destination task is invalid!\n");

    u16EvtFirst = sg_u16EvtFirst;

	sg_u16EvtFirst = (sg_u16EvtFirst + 1) % MAX_QUEUE_NUM; /* Update the position of first available event */
    sg_u16EvtCnt--;                                            /* Update the event count */
                                     
    /* Get the evenet information */
    return &sg_atEvtQueue[u16EvtFirst];
}

/******************************************************************************
* Name       : void Event_Remove(void)
* Function   : Remove the processed event
******************************************************************************/
void Event_Remove(void)
{  
	sg_u16EvtFirst = (sg_u16EvtFirst + 1) % MAX_QUEUE_NUM; /* Update the position of first available event */
    sg_u16EvtCnt--;                                            /* Update the event count */

    return;
}

/* end of file */


