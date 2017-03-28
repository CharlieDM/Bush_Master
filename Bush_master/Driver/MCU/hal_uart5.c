
#include "hal_uart5.h"
#include "stm32f10x_usart.h"
#include "core_hal.h"

#if UART5_FIFO_EN == 1

typedef void (*Rxd5Func)(uint8_t data);

static void Uart5_NVICConfig(void);
static void Uart5_Send(uint8_t *TBuf, uint16_t _usLen);
static void Hal_RS485_IOInit(void);
static void Usart5Dummy(uint8_t data){;}	
static Rxd5Func Rxd5 = Usart5Dummy;

static void Register(void *rxdFucn)
{
    Rxd5 = (Rxd5Func)rxdFucn;
}
	
void Hal_Uart5_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	/* 串口5 TX = PC12   RX = PD2 */
	/* 第1步： 开启GPIO和UART时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

	/* 第2步：将USART Tx的GPIO配置为推挽复用模式 */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* 第3步：将USART Rx的GPIO配置为浮空输入模式
		由于CPU复位后，GPIO缺省都是浮空输入模式，因此下面这个步骤不是必须的
		但是，我还是建议加上便于阅读，并且防止其它地方修改了这个口线的设置参数
	*/
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);


	/* 第4步： 配置串口硬件参数 */
	USART_InitStructure.USART_BaudRate            = UART5_BAUD;	/* 波特率 */
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART5, &USART_InitStructure);
	/*打开空闲中断，即接收到一帧数据完之后改中断才发生*/
	USART_ITConfig(UART5, USART_IT_IDLE, ENABLE);	/* 使能接收中断 */
	/*有数据来该中断就发生*/
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);	/* 使能接收中断 */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		注意: 不要在此处打开发送中断
		发送中断使能在SendUart()函数打开
	*/
	USART_Cmd(UART5, ENABLE);		/* 使能串口 */

	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(UART5, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */
	
	Hal_RS485_IOInit();
	Uart5_NVICConfig();
	
	Device.Usart5.Send = Uart5_Send;
	Device.Usart5.Register = Register;
}

static void Hal_RS485_IOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_RS485_TXEN, ENABLE);	  /* 打开GPIO时钟 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;	/* 推挽输出模式 */
	GPIO_InitStructure.GPIO_Pin   = PIN_RS485_TXEN;
	GPIO_Init(PORT_RS485_TXEN, &GPIO_InitStructure);
}

static void Uart5_NVICConfig(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	/* 使能串口5中断 */
	NVIC_InitStructure.NVIC_IRQChannel            = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

static void Uart5_Send(uint8_t *TBuf, uint16_t _usLen)
{
	uint8_t i_com=0;

	RS485_TX_EN(); 
	for(i_com=0;i_com<_usLen;i_com++)
	{
		USART_SendData(UART5,TBuf[i_com]);
		while(USART_GetITStatus(UART5, USART_FLAG_TXE) == RESET);
	}
	RS485_RX_EN();
}

void UART5_IRQHandler(void)
{
	uint8_t ch;
	/* 处理接收中断  */
	if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
	{
		ch = USART_ReceiveData(UART5);	
		Rxd5(ch);		
	}
}
#endif





