
#include "core_hal.h"
#include "hal_gpio.h"
#include "hal_uart3.h"
#include "hal_uart5.h"
#include "hal_spi.h"
#include "hal_systick.h"
#include "modbus_slave.h"

DeviceStruct Device;

uint8 HAL_Init(void)
{
	Hal_SystickInit();
	Hal_Uart5_Init();
	Hal_Usart3_Init();
	Hal_Spi_Init();	
	Hal_GpioInit();
	ModSlaveInit();
	
	return SW_OK;
}

/* end of file */

