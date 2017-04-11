
#include "type_def.h"
#include "debug.h"
#include "Task.h"

/* Add your task process here */
const PF_TASK_PROCESS cg_apfTaskFn[] = 
{
    Task_Process
};

/* Calculate the total task number */
const uint8 cg_u8TaskNum = sizeof(cg_apfTaskFn)/sizeof(PF_TASK_PROCESS);

/* End of file */


