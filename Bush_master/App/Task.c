
#include "type_def.h"
#include "task.h"
#include "debug.h"
#include "Core_os.h"
#include "Core_event.h"

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


