
#include "core_hal.h"
#include "core_os.h"

int main(void)
{   
	Os_Init(NULL);
	OS_Run();	
}
