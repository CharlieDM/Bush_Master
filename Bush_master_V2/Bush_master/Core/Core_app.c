
#include "debug.h"
#include "appdata.h"
#include "Core_app.h"
#include "task.h"

DataStruct Data = {{DEFAULT_SENSOR,DEFAULT_SENSOR,DEFAULT_SENSOR,DEFAULT_SENSOR,DEFAULT_SENSOR}};

/* Add your task process here */
const PF_TASK_PROCESS cg_apfTaskFn[] = 
{
    Task_Process
};

/* Calculate the total task number */
const uint8 cg_u8TaskNum = sizeof(cg_apfTaskFn)/sizeof(PF_TASK_PROCESS);

void AppDataInit(void)
{
	
}

/* End of file */


