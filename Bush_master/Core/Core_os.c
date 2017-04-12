
#include "type_def.h"
#include "Core_os.h"
#include "debug.h"
#include "Core_Hal.h"
#include "Core_Event.h"

static PF_POLL   sg_pfPoll   = NULL;
static EventStruct sg_tEvt = {NULL};
static EventStruct *sg_ptEvt = &sg_tEvt;

/******************************************************************************
* Name       : uint8 OS_Init(PF_POLL pfPoll)
* Function   : Init MOE
******************************************************************************/
uint8 OS_Init(PF_POLL pfPoll)
{
    /* Get poll process function */
    sg_pfPoll = pfPoll;

    /* Init HAL */
    HAL_Init();

	/* Init Data */
	AppDataInit();
	
    /* Init event mechanism */
    Event_Init();
    
    /* Init all tasks */
    sg_tEvt.u8Evt  = EVENT_INIT;
    for(sg_tEvt.u8Task = 1; sg_tEvt.u8Task <= MAX_TASK_NUM; sg_tEvt.u8Task++)
    {
        cg_apfTaskFn[sg_tEvt.u8Task - 1](sg_tEvt.u8Evt, sg_tEvt.pPara);
    }
   
    sg_tEvt.u8Task = TASK_NO_TASK;
    sg_tEvt.u8Evt  = EVENT_NONE;

    return SW_OK;
}

/******************************************************************************
* Name       : void Moe_Run_System(void)
* Function   : The main function to schedule tasks.
******************************************************************************/
void OS_Run(void)
{
    while(1)                             
    {
    	/* if have the Event, then run the Event Function  */
        if(sg_ptEvt = Event_Get())    
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
