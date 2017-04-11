/******************************************************************************
* File       : Task1.c
* Function   : It is No. 1 task which is just a demo task
* Description: To be done.           
* Version    : V1.00
* Author     : Ian
* Date       : 3rd May 2016
* History    :  No.  When           Who           What
*               1    3/May/2016     Ian           Create
******************************************************************************/

#include "type_def.h"
#include "Task.h"
#include "debug.h"

static uint8 sg_u8TaskID = TASK_NO_TASK;

/******************************************************************************
* Name       : uint8 Task1_Process(uint8 u8Evt, void *pPara)
* Function   : Task 1 process
******************************************************************************/
uint8 Task_Process(uint8 u8Evt, void *pPara)
{   
    static uint32 u32Temp = 0x0;
    u32Temp++;
    /* Check which event should be processed */
    switch (u8Evt)
    {
        /* If it is a test event */
        case EVENT_TEST:       
        {
            
            return SW_OK;     /* Return SW_OK to indicate event is processed */
        }

        /* If it is other event */
        default:       
        {
            return u8Evt;     /* Return event to indicate event is NOT processed */
        }
    }
}

/* End of file */


