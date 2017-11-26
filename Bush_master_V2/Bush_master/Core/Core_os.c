#include "debug.h"
#include "Core_Hal.h"
#include "Core_os.h"
#include "appdata.h"
#include "app.h"
#include "modbus_slave.h"

static PF_POLL   sg_pfPoll   = APP_Run;
DataStruct Data = {{0}};
/******************************************************************************
* Name       : uint8 OS_Init(void)
* Function   : Init OS
******************************************************************************/
uint8 Os_Init(void)
{
    /* Init HAL */
    HAL_Init();

	APP_Init();

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
		/* run the periodic task */
		Device.Systick.Run();
		
		ModSlavePoll();
		/* aways running task  */
		if (sg_pfPoll)
		{
			sg_pfPoll();                 
		}
    }
}

/* End of file */
