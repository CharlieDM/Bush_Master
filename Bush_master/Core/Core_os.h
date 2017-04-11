
#ifndef _CORE_OS_H_
#define _CORE_OS_H_

#ifdef __cplusplus
extern "C" {
#endif

#define Firmware_VERSION       "Bush V0.11"

typedef void (*PF_POLL)(void);

/* Task MACRO */
#define TASK_NO_TASK                (0x00)                    /* Task number which means there is no task */
#define TASK_ALL_TASK               (0xFF)                    /* Task number which means all tasks        */
#define TASK_FIRST_TASK             (0x01)                    /* Task number of the first one             */
#define TASK_LAST_TASK              MAX_TASK_NUM              /* Task number of the last one              */

/******************************************************************************
* Name       : uint8 Os_Init(PF_POLL pfPoll)
* Function   : Init MOE
******************************************************************************/
uint8 Os_Init(PF_POLL pfPoll);


/******************************************************************************
* Name       : void Os_Run(void)
* Function   : The main function to schedule tasks.
******************************************************************************/
void Os_Run(void);

#ifdef __cplusplus
}
#endif

#endif /* _MOE_CORE_H_ */

/* End of file */
