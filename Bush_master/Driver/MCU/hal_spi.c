
#include "hal_spi.h"
#include "core_hal.h"
#include "type_def.h"

/* ----------------------- Static Function ------------------------------------------*/
static void Hal_Spi_Write_Read(uint8 len,uint8 *wbuf, uint8 * rbuf);


void Hal_Spi_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_SPI1, ENABLE);	

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 

	GPIO_InitStructure.GPIO_Pin = PIN_NSS1;	
	GPIO_Init(PORT_SPI1, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_SCK1;
	GPIO_Init(PORT_SPI1, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_MOSI1;
	GPIO_Init(PORT_SPI1, &GPIO_InitStructure);	
	
	SPI1_NSS_1();	
	SPI1_MOSI_0();
	
#if SPI1_CPOL==0
	SPI1_SCK_0(); 
#endif

#if SPI1_CPOL==1
	SPI1_SCK_1();  
#endif

	Device.Spi.ReadWrite = Hal_Spi_Write_Read;
}

static void Hal_Spi_Deay(uint16 _nus)
{
	uint32 ui;
	while(_nus--)
	{
		for( ui = 0; ui < 5; ui++ );
	}
}

static void Hal_Spi_Write_Read(uint8 len,uint8 *wbuf, uint8 * rbuf)
{
	uint8_t  i = 0;
	uint8_t  wtemp = 0;
	uint8_t  rtemp = 0;	
	
	Enter_Critical();
	
	SPI1_MOSI_0(); 
	SPI1_NSS_0();
	Hal_Spi_Deay(600);
	for(i = 0; i < len; i++)
	{
	SPI1_SCK_0();	
	if(i%8 == 0)
	{
		wtemp = wbuf[i/8];
	}
	if (wtemp & 0x01)
	{
		SPI1_MOSI_1();
	}
	else
	{
		SPI1_MOSI_0();
	}
	wtemp >>= 1;
	Hal_Spi_Deay(600);
	SPI1_SCK_1();	

	Hal_Spi_Deay(600);
	rtemp >>= 1;
	if(MISO1_IS_HIGH())
	{
		rtemp |= 0x80;
	}
	else
	{
		rtemp |= 0x80;
	}
	if(((i+1)%8 == 0)&&(i != 0))
	{
		rbuf[(i+1)/8-1] = rtemp;
	}
	}
	
	Hal_Spi_Deay(600);
	
	SPI1_SCK_0();
	SPI1_NSS_1();	
	SPI1_MOSI_0(); 
	
	Leave_Critical();
}






