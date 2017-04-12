
/* ----------------------- Platform includes --------------------------------*/
#include "bittool.h"

void U8CpyToU16(int16 *psbuf, uint8 *pucbuf, uint8 uclen)
{
	uint8 uci = 0;
	for(uci = 0; uci<uclen/2; uci++)
	{
		psbuf[uci] = pucbuf[2*uci]*256 + pucbuf[2*uci+1];
	}
}

