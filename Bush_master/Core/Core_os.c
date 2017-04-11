
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
    while(1)                             /* The main loop                */
    {
        if (sg_pfPoll)
        {
            sg_pfPoll();                 /* Do polling process if needed */
        }

        if(sg_ptEvt = Event_Get())   /* Check events                 */
        {
            cg_apfTaskFn[sg_ptEvt->u8Task - 1](sg_ptEvt->u8Evt, sg_ptEvt->pPara); /* Call the task process function */
            sg_ptEvt->u8Task = TASK_NO_TASK;                     /* Finish task processing and cancel active task mark */
            sg_ptEvt = &sg_tEvt;                                 /* Point to a none event & task "event struct"        */   
        }
		
		Device.Systick.Run();
    }
}

/* End of file */
