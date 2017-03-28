#ifndef __APP__H
#define __APP__H
#include "bsp_global.h"

//485用定时器
#define TIMER_Read_485                1
#define TIMER_Cmd_485                 2
#define TIMER_Cmd_Sleep_485           3
#define TIMER_RT_Condition              10//对空调操作计时器标志
#define TIMER_Screen0_Text_Update       11//屏幕0传感器值计时器标志
#define TIMER_Screen1_Text_Update       12//屏幕1传感器值
#define TIMER_Screen_Fault_Update       13//报错码
#define TIMER_Screen0_State_Update      5//工作状态
#define TIMER_Screen1_State_Update      15//自动模式下的按键指示
#define TIMER_Screen0_Feel_Update       16//舒适度

//新风机，排风机计时器
#define TIMER_XingFeng_MASTER    17
#define TIMER_HEAT_MASTER        18
#define TIMER_HEAT_XingFeng      19
#define TIMER_OPEN_CLOSE         20
//新风阀计时器任务排序
#define TIMER_XingFengFa_OPEN    21
#define TIMER_XingFengFa_CLOSE   22

extern uint16_t Din_16bit;
extern uint16_t Dout_12bit;

extern void    Init_Hard(void);
extern void    Init_Soft(void);
extern void    Uart_ReciveProcess(void);
extern void    Uart_SendProcess(void);
extern void    Control_Task(void);
extern void    CO2_Sample(void);
extern void    AM2301_Sample(void);
extern void    SET_Button_State(uint8_t page,uint8_t addr,uint8_t state);
extern uint8_t SPI_CheckSum(uint8_t *buf,uint8_t cnt);
extern uint8_t Room_CheckSum(uint8_t *buf,uint8_t cnt);
extern void    Room_Board_Write(uint8_t target,uint8_t addr,uint8_t cmd);
extern void    Write_Air_Condition(void);
#endif
