
#ifndef _CORE_OS_H_
#define _CORE_OS_H_

#include "type_def.h"
	
#define Firmware_VERSION       "Bush V0.11"

typedef void (*PF_POLL)(void);

/* Task MACRO */
#define TASK_NO_TASK                (0x00)                    /* Task number which means there is no task */
#define TASK_ALL_TASK               (0xFF)                    /* Task number which means all tasks        */
#define TASK_FIRST_TASK             (0x01)                    /* Task number of the first one             */
#define TASK_LAST_TASK              MAX_TASK_NUM              /* Task number of the last one              */

uint8 Os_Init(PF_POLL pfPoll);
void Os_Run(void);


#endif /* _MOE_CORE_H_ */

/* End of file */

