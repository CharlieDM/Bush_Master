
#include "stm32f10x.h"
#include "hal_systick.h"
#include "core_hal.h"
#include "debug.h"

#define Task_Num			(20)

typedef struct _TaskStruct
{
	uint8 Run;
	uint8 Timer;
	uint8 LvTimer;
	void (*FuncHander)();
}TaskStruct;

void Func(){};

static uint8 TaskPos = 0;	
static TaskStruct TaskComps[Task_Num] = {{0,100,100,Func}};

static void RegisterSystick(uint8 timer, void(*Func)())
{
	
	CHECK_IF_RET_VOID(TaskPos>19, "Invalid Pos!!\n");
	
	TaskComps[TaskPos].Run = 0;
	TaskComps[TaskPos].Timer = timer;
	TaskComps[TaskPos].LvTimer = timer;
	TaskComps[TaskPos].FuncHander = Func;
}

static void TaskCheck(void)
{
    uint8 i;
    for (i=0; i<TaskPos; i++)          
    {
         if (TaskComps[i].Timer)          
        {
            TaskComps[i].Timer--;          
            if (TaskComps[i].Timer == 0)        
            {
                 TaskComps[i].Timer = TaskComps[i].LvTimer;       
                 TaskComps[i].Run = 1;           
            }
        }
   }
}

static void TaskProcess(void)
{
    uint8 i;
    for (i=0; i<TaskPos; i++)            
    {
         if (TaskComps[i].Run)            
        {
             TaskComps[i].FuncHander();         
             TaskComps[i].Run = 0;           
        }
    }   
}

void SysTick_ISR(void)
{
	TaskCheck();
}

void Hal_SystickInit(void)
{
	SysTick_Config(SystemCoreClock / 100);
	Device.Systick.Register = RegisterSystick;
	Device.Systick.Run = TaskProcess;
}


