
#ifndef _CORE_EVENT_H_
#define _CORE_EVENT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* MACRO */
#define EVENT_URGENT            (1)                       /* It is an urgent event */
#define EVENT_NORMAL            (0)                       /* It is a normal event  */

#define MAX_TASK_NUM             (3)                  /* Max number of tasks */
#define MAX_QUEUE_NUM			 (MAX_TASK_NUM*10)

#define EVENT_NONE                  (0x00)                    /* There is no events                       */
#define EVENT_INIT                  (0x01)                    /* There is a test event                    */
#define EVENT_TIMER                 (0x02)                    /* There is a timer event                   */
#define EVENT_MSG                   (0x03)                    /* There is a message event                 */
#define EVENT_INT                   (0x04)                    /* There is an interrupt event              */
#define EVENT_DELAY                 (0x05)                    /* There is a deley event                   */
#define EVENT_PERIODIC              (0x06)                    /* There is a periodic call event           */
#define EVENT_TEST                  (0x07)                    /* There is a test event                    */
#define EVENT_BREAK                 (0x08)                    /* There is an event after a break          */

typedef struct _Event
{
    void *pPara;
    uint8 u8Task;
    uint8 u8Evt;
}EventStruct;

uint8 Event_Init(void);
uint8 Event_Set(uint8 u8TaskID, uint8 u8Evt, uint8 u8Urg, void *pPara);
EventStruct* Event_Get(void);


#ifdef __cplusplus
}
#endif

#endif /* _MOE_EVENT_H_ */

/* End of file */

