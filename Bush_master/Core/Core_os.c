
#include "debug.h"
#include "Core_Hal.h"
#include "Core_Event.h"
#include "Core_app.h"
#include "Core_os.h"

static PF_POLL   sg_pfPoll   = NULL;
static EventStruct sg_tEvt = {NULL};
static EventStruct *sg_ptEvt = &sg_tEvt;
extern PF_TASK_PROCESS cg_apfTaskFn[];
/******************************************************************************
* Name       : uint8 OS_Init(void)
* Function   : Init OS
******************************************************************************/
uint8 Os_Init(void)
{
    /* Init HAL */
    HAL_Init();

	/* Init Data */
	AppDataInit();
	
    /* Init event mechanism */
    Event_Init();
    
    /* Init all tasks */
	/*
    sg_tEvt.u8Evt  = EVENT_INIT;
    for(sg_tEvt.u8Task = 1; sg_tEvt.u8Task <= MAX_TASK_NUM; sg_tEvt.u8Task++)
    {
        cg_apfTaskFn[sg_tEvt.u8Task - 1](sg_tEvt.u8Evt, sg_tEvt.pPara);
    }
    */
   
    sg_tEvt.u8Task = TASK_NO_TASK;
    sg_tEvt.u8Evt  = EVENT_NONE;

    return SW_OK;
}

/******************************************************************************
* Name       : void Os_Run(void)
* Function   : The main function to schedule tasks.
******************************************************************************/
void Os_Run(void)
{
    while(1)                             
    {
    	/* if have the Event, then run the Event Function  */
		sg_ptEvt = Event_Get();
        if(sg_ptEvt->u8Task)    
        {
            cg_apfTaskFn[sg_ptEvt->u8Task - 1](sg_ptEvt->u8Evt, sg_ptEvt->pPara);  
            sg_ptEvt->u8Task = TASK_NO_TASK;                      
            sg_ptEvt = &sg_tEvt;                                    
         }

		/* run the periodic task */
		Device.Systick.Run();

		/* aways running task  */
		if (sg_pfPoll)
		{
			sg_pfPoll();                 
		}
    }
}

/* End of file */
