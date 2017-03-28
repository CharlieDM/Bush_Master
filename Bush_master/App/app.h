#ifndef __APP__H
#define __APP__H
#include "bsp_global.h"

//485�ö�ʱ��
#define TIMER_Read_485                1
#define TIMER_Cmd_485                 2
#define TIMER_Cmd_Sleep_485           3
#define TIMER_RT_Condition              10//�Կյ�������ʱ����־
#define TIMER_Screen0_Text_Update       11//��Ļ0������ֵ��ʱ����־
#define TIMER_Screen1_Text_Update       12//��Ļ1������ֵ
#define TIMER_Screen_Fault_Update       13//������
#define TIMER_Screen0_State_Update      5//����״̬
#define TIMER_Screen1_State_Update      15//�Զ�ģʽ�µİ���ָʾ
#define TIMER_Screen0_Feel_Update       16//���ʶ�

//�·�����ŷ����ʱ��
#define TIMER_XingFeng_MASTER    17
#define TIMER_HEAT_MASTER        18
#define TIMER_HEAT_XingFeng      19
#define TIMER_OPEN_CLOSE         20
//�·緧��ʱ����������
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
