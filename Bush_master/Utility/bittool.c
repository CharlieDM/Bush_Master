
/* ----------------------- Platform includes --------------------------------*/
#include "bittool.h"

void U8CpyToU16(uint16 *psbuf, uint8 *pucbuf, uint8 uclen)
{
	uint8 uci = 0;
	for(uci = 0; uci<uclen/2; uci++)
	{
		psbuf[uci] = pucbuf[2*uci]*256 + pucbuf[2*uci+1];
	}
}

void U16CpyToU8(uint8 * pucbuf, uint16 *pusbuf, uint8 uclen)
{
  uint8 uci = 0;
  for(uci = 0; uci < uclen; uci++ )
  {
	  pucbuf[2*uci] = (uint8)(pusbuf[uci]>>8);
	  pucbuf[2*uci+1] = (uint8)(pusbuf[uci]&0xFF);
  }
}


