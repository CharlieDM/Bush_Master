#include "ads1232.h"


void Delay_us(uint32_t cnt)
{
	uint32_t i = 0;
	for(i=0;i<cnt;i++);
}

void ADS1232_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		//初始化IO
		RCC_APB2PeriphClockCmd(RCC_ADS1232_1_GROUP1, ENABLE);

	  //配置输出引脚
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;	/* 推挽输出模式 */
		
		GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_1_A0;
		GPIO_Init(GPIO_PORT_1_A0, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_1_CLK;
		GPIO_Init(GPIO_PORT_1_CLK, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_1_PDWN;
		GPIO_Init(GPIO_PORT_1_PDWN, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_1_GAIN1;
		GPIO_Init(GPIO_PORT_1_GAIN1, &GPIO_InitStructure);
		
		//必须重映射
	  GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST,ENABLE);
		
		GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_1_GAIN0;
		GPIO_Init(GPIO_PORT_1_GAIN0, &GPIO_InitStructure);

		RCC_APB2PeriphClockCmd(RCC_ADS1232_1_GROUP1, ENABLE);
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;	/* 浮空输入模式 */
		GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_1_DOUT;
		GPIO_Init(GPIO_PORT_1_DOUT, &GPIO_InitStructure);
		
		//ADS1232复位
    PDWN_1_OFF;
    Delay_ms(10); 
    PDWN_1_ON;
    Delay_ms(10); 
		
    //时钟默认低电平
    SCLK_1_OFF;
    //ADS1232增益选择为1
    GAIN0_1_OFF;
		GAIN1_1_OFF;
		//选择通道1
		A0_1_OFF;
    Delay_ms(10); 
    //复位后校准
    ADS1232_Read(0x01);
    ADS1232_Read(0x00);
}

//Calibration:0x01:校正,0x00:不校正
int32_t ADS1232_Read(unsigned char Calibration)
{
    unsigned char   loop=0x00;
    signed   long   Result=0x00;
    for (loop=0x00;loop<24;loop++)  //循环读取数据，发送24个脉冲
    { 
			  Result = Result<<1;
        SCLK_1_ON;
			  if(DOUT)
				{
					Result |= 0x01;
				}
				else
				{
					Result &=~0x01;
				}
			  Delay_us(100); 
        SCLK_1_OFF;
			  Delay_us(100);
    }
    SCLK_1_ON;
		Delay_us(100);
    SCLK_1_OFF;
		Delay_us(100);
    if(Calibration)
    {
      SCLK_1_ON;
			Delay_us(100);
      SCLK_1_OFF;
      Delay_us(100);				
      while(DOUT == 0);
      while(DOUT == 1);
    }
    else
    {
      while(DOUT == 0);
			while(DOUT == 1);
    }                  
    if(Result>=NEGATIVE)            //表示负值,define NEGATIVE 0x800000
    {                               //define EFFECTIVE 0x7FFFFF Result |= 0xff000000
          Result |= 0xff000000;
    }
    return Result;
}
